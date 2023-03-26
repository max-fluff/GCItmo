#include "Player.h"
#include "../../Game.h"
#include "../../RenderComponents/GameComponent.h"
#include "../Shaders/DepthShader.h"
#include "../Models/Model3D.h"
#include <memory>


Player::Player(std::shared_ptr<Game> game, float _r, float _axisAngle = 3.0f, float _orbitAngle = 0.5f,
               float _orbitR = 0.0f, LPCWSTR _filepath = L"", Player* parent = nullptr)
{
	this->game = game;
	this->filePath = _filepath;
	this->r = _r;
	this->parent = parent;
	this->material = std::make_shared<Material>(DirectX::SimpleMath::Vector4(0.8f, 0.8f, 0.8f, 100.0f));
}

void Player::Initialize()
{
	sphereModel = new Model3D(game, "MyGame/Models/can.fbx", L"Textures\\can.png", Vector3(0, 0, 0));
	sphereModel->Initialize();
	shader = std::make_shared<SphereShader>(game, this, filePath);
	shader->Initialize(L"./Shaders/SphereShader.hlsl");
	depthShader = new DepthShader(game);
	depthShader->Initialize(L"./Shaders/DepthShader.hlsl");
	collider.Center = pos;
	collider.Radius = r;
	pos.y = -scale * r;

	for (auto gc : game->gameComponents)
	{
		Model3D* cubic = dynamic_cast<Model3D*>(gc.get());
		if (cubic)
			models.push_back(cubic);
	}
}

void Player::Render()
{
	//
	modelMat = scaleMat * spinMat * translationMat;

	data.worldViewProj = modelMat *
		game->camera->GetViewMatrix() *
		game->camera->GetProjectionMatrix();

	data.worldInverseT = DirectX::XMMatrixTranspose(
		DirectX::XMMatrixInverse(nullptr,
		                         modelMat));

	data.world = modelMat;

	data.ambient = game->dirLight.get()->ambient;
	data.light = game->dirLight.get()->light;
	data.specular = material->specularCoef;
	data.direction = game->dirLight.get()->direction;

	data.eyePos = DirectX::SimpleMath::Vector4(game->camera->pos.x, game->camera->pos.y, game->camera->pos.z, 1.0f);

	shader->Render();
	collider.Center = pos;
	for (auto cubic : models)
	{
		if (collider.Intersects(cubic->collider) || cubic->isSticked)
		{
			if (r * scale >= cubic->collider.Extents.x && r * scale >= cubic->collider.Extents.y / 2 && r * scale >=
				cubic->collider.Extents.z)
			{
				cubic->playerCoords = spinMat * translationMat;
				if (!cubic->isSticked)
				{
					cubic->pos = cubic->pos - pos;
					cubic->worldMatrix = DirectX::XMMatrixScaling(cubic->scale.x, cubic->scale.y, cubic->scale.z) *
						DirectX::XMMatrixTranslation(cubic->pos.x, cubic->pos.y, cubic->pos.z) *
						DirectX::XMMatrixInverse(nullptr, spinMat);
					cubic->isSticked = true;
					targetScale += 0.2f;
					game->cameraController->targetArm += 0.2f;
				}
			}
		}
	}

	scaleMat = DirectX::XMMatrixScaling(scale, scale, scale);
	collider.Radius = scale * r * 0.8f;
	translationMat = DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z);
}

void Player::MainPass()
{
	modelMat = scaleMat * spinMat * translationMat;

	data.worldViewProj = modelMat *
		game->camera->GetViewMatrix() *
		game->camera->GetProjectionMatrix();

	data.worldInverseT = DirectX::XMMatrixTranspose(
		DirectX::XMMatrixInverse(nullptr,
		                         modelMat));

	data.lightViewProj =
		game->dirLight->viewMatrix *
		game->dirLight->orthoMatrix;

	data.world = modelMat;

	data.worldView = DirectX::XMMatrixIdentity() * game->camera->GetViewMatrix();

	data.ambient = game->dirLight.get()->ambient;
	data.light = game->dirLight.get()->light;
	data.specular = material->specularCoef;
	data.direction = game->dirLight.get()->direction;

	data.eyePos = DirectX::SimpleMath::Vector4(game->camera->pos.x, game->camera->pos.y, game->camera->pos.z, 1.0f);
	data.viewProj = game->dirLight->viewMatrix * game->dirLight->orthoMatrix;


	D3D11_MAPPED_SUBRESOURCE mappedResource;
	auto helper = shader->constantBuffer;
	auto help2 = helper->buffer.Get();

	HRESULT res = game->context->Map(help2, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	CopyMemory(mappedResource.pData, &data, sizeof(CB_ModelLightning));
	game->context->Unmap(shader->constantBuffer->buffer.Get(), 0);
	game->context->VSSetConstantBuffers(0, 1, shader->constantBuffer->buffer.GetAddressOf());
	game->context->PSSetConstantBuffers(0, 1, shader->constantBuffer->buffer.GetAddressOf());


	D3D11_VIEWPORT viewport = {};
	viewport.Width = static_cast<float>(800);
	viewport.Height = static_cast<float>(800);
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.MinDepth = 0;
	viewport.MaxDepth = 1.0f;

	game->context->RSSetViewports(1, &viewport);

	game->context->IASetInputLayout(shader->layout.Get());
	game->context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	game->context->PSSetShaderResources(0, 1, shader->myTexture.GetAddressOf());
	game->context->PSSetShaderResources(1, 1, &game->shadowResourceView);
	game->context->VSSetShader(shader->vertexShader.Get(), nullptr, 0);
	game->context->PSSetShader(shader->pixelShader.Get(), nullptr, 0);
	game->context->PSSetSamplers(0, 1, shader->samplerState.GetAddressOf());
	game->context->PSSetSamplers(1, 1, shader->samplerDepthState.GetAddressOf());
	//add
	game->context->IASetIndexBuffer(shader->indexBuffer->buffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	game->context->IASetVertexBuffers(0, 1, shader->vertexBuffer->buffer.GetAddressOf(),
	                                  shader->strides, shader->offsets);
	//game->context->OMSetDepthStencilState(game->depthStencilState.Get(), 0);

	game->context->DrawIndexed(shader->index.size(), 0, 0);

	game->context->PSSetShader(0, nullptr, 0);
	game->context->VSSetShader(0, nullptr, 0);


	collider.Center = pos;
	for (auto cubic : models)
	{
		if (collider.Intersects(cubic->collider) || cubic->isSticked)
		{
			if (r * scale >= cubic->collider.Extents.x && r * scale >= cubic->collider.Extents.y / 2 && r * scale >=
				cubic->collider.Extents.z)
			{
				cubic->playerCoords = spinMat * translationMat;
				if (!cubic->isSticked)
				{
					cubic->pos = cubic->pos - pos;
					cubic->worldMatrix = DirectX::XMMatrixScaling(cubic->scale.x, cubic->scale.y, cubic->scale.z) *
						DirectX::XMMatrixTranslation(cubic->pos.x, cubic->pos.y, cubic->pos.z) *
						DirectX::XMMatrixInverse(nullptr, spinMat);
					cubic->isSticked = true;
					targetScale += 0.2f;
					game->cameraController->targetArm += 0.2f;
				}
			}
		}
	}

	scaleMat = DirectX::XMMatrixScaling(scale, scale, scale);
	collider.Radius = scale * r * 0.8f;
	translationMat = DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z);
}

void Player::DepthPass()
{
	modelMat = scaleMat * spinMat * translationMat;

	data.worldViewProj = modelMat *
		game->camera->GetViewMatrix() *
		game->camera->GetProjectionMatrix();

	data.worldInverseT = DirectX::XMMatrixTranspose(
		DirectX::XMMatrixInverse(nullptr,
		                         modelMat));

	data.lightViewProj =
		game->dirLight->viewMatrix *
		game->dirLight->orthoMatrix;

	data.world = modelMat;

	data.worldView = DirectX::XMMatrixIdentity() * game->camera->GetViewMatrix();

	data.ambient = game->dirLight.get()->ambient;
	data.light = game->dirLight.get()->light;
	data.specular = material->specularCoef;
	data.direction = game->dirLight.get()->direction;

	data.eyePos = DirectX::SimpleMath::Vector4(game->camera->pos.x, game->camera->pos.y, game->camera->pos.z, 1.0f);
	data.viewProj = game->dirLight->viewMatrix * game->dirLight->orthoMatrix;


	D3D11_MAPPED_SUBRESOURCE mappedResource;
	auto helper = shader->constantBuffer;
	auto help2 = helper->buffer.Get();
	HRESULT res = game->context->Map(help2, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	CopyMemory(mappedResource.pData, &data, sizeof(CB_ModelLightning));
	game->context->Unmap(shader->constantBuffer->buffer.Get(), 0);
	game->context->VSSetConstantBuffers(0, 1, shader->constantBuffer->buffer.GetAddressOf());
	game->context->PSSetConstantBuffers(0, 1, shader->constantBuffer->buffer.GetAddressOf());
	//game->context->GSSetConstantBuffers(0, 1, shader->constantBuffer->buffer.GetAddressOf());

	D3D11_VIEWPORT viewport = {};
	viewport.Width = static_cast<float>(game->SHADOWMAP_WIDTH);
	viewport.Height = static_cast<float>(game->SHADOWMAP_HEIGHT);
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.MinDepth = 0;
	viewport.MaxDepth = 1.0f;

	game->context->RSSetViewports(1, &viewport);

	game->context->IASetInputLayout(depthShader->layout.Get());
	game->context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	game->context->VSSetShader(depthShader->vertexShader.Get(), nullptr, 0);
	//game->context->GSSetShader(depthShader->geomShader.Get(),nullptr,0);
	//game->context->PSSetShader(depthShader->pixelShader.Get(), nullptr, 0);

	game->context->IASetVertexBuffers(0, 1, shader->vertexBuffer->buffer.GetAddressOf(),
	                                  shader->strides, shader->offsets);
	game->context->IASetIndexBuffer(shader->indexBuffer->buffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	game->context->DrawIndexed(shader->indexBuffer->size, 0, 0);
	//game->context->DrawInstanced(shader->indexBuffer->size,1,0, 0);
	//game->context->GSSetShader(nullptr,nullptr,0);
}


void Player::Update(float deltaSec)
{
	if (scale < targetScale)
	{
		scale += deltaSec * 0.1f;
	}
	pos.y = +scale * r;
}
