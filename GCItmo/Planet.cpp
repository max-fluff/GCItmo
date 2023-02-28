#include "Planet.h"
#include "Game.h"
#include "WICTextureLoader.h"


Planet::Planet(std::shared_ptr<Game> game, float radius, float axisAngle = 3.0f, float orbitAngle = 0.5f,
               float orbitRadius = 0.0f, LPCWSTR filepath = L"")
{
	this->game = game;
	this->axeAngle = axisAngle;
	this->orbitAngle = orbitAngle;
	this->filePath = filepath;
	this->orbitR = orbitRadius;
	this->r = radius;
}

void Planet::Draw()
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	HRESULT res = game->context->Map(constantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	CopyMemory(mappedResource.pData, &game->worldViewData, sizeof(CB_VS_vertexshader));
	game->context->Unmap(constantBuffer.Get(), 0);
	game->context->VSSetConstantBuffers(0, 1, constantBuffer.GetAddressOf());


	game->context->OMSetRenderTargets(1, &game->renderTargetView, game->depthStencilView);

	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));

	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

	HRESULT hr = game->device->CreateDepthStencilState(&depthStencilDesc, &(game->depthStencilState));

	DirectX::XMMATRIX a = game->camera->GetViewMatrix();
	shape->SetDepthBufferMode(false);

	shape->Draw(modelMat * rotationAxisMat * DirectX::XMMatrixTranslation(orbitR, 0.0f, 0.0f) * rotationOrbitMat,
		game->camera->GetViewMatrix(), game->camera->GetProjectionMatrix(), DirectX::Colors::White,
		texture.Get(), false);
	game->context->OMSetRenderTargets(0, nullptr, nullptr);
}

void Planet::Update(float deltaTime)
{
	rotationAxisMat *= DirectX::XMMatrixRotationY(axeAngle * 0.01f);
	rotationOrbitMat *= DirectX::XMMatrixRotationY(orbitAngle * 0.01f);
}

void Planet::Init()
{
	D3D11_BUFFER_DESC constantBufDesc;
	constantBufDesc.Usage = D3D11_USAGE_DYNAMIC;
	constantBufDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constantBufDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	constantBufDesc.MiscFlags = 0;
	constantBufDesc.ByteWidth = sizeof(CB_VS_vertexshader) + (16 - sizeof(CB_VS_vertexshader)) % 16;
	constantBufDesc.StructureByteStride = 0;

	game->device->CreateBuffer(&constantBufDesc, 0, constantBuffer.GetAddressOf());


	HRESULT res = DirectX::CreateWICTextureFromFile(game->device.Get(), filePath, nullptr, texture.GetAddressOf());

	shape = DirectX::GeometricPrimitive::CreateSphere(game->context, r, 32, false, true);
}


DirectX::BoundingBox Planet::GetBox()
{
	return boundingBox;
}
