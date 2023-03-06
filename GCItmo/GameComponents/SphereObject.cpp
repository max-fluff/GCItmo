#include "SphereObject.h"

#include <corecrt_math_defines.h>

#include "../Game.h"
#include "../DXSDK/WICTextureLoader.h"
#include "../GameComponents/Planet.h"

SphereObject::SphereObject(Game* game, Planet* gc, LPCWSTR filepath)
{
	this->game = game;
	this->gc = gc;
	this->filepath = filepath;
}

void SphereObject::GenerateSphere(float r, int segments, int slices)
{
	int numSlices = slices;
	int numSegments = segments;
	int step = 0;

	for (float verticalAngle = -M_PI / 2.0f + M_PI / (2.0f * (numSlices * 1.0f)); verticalAngle < M_PI / 2.0f;
	     verticalAngle += M_PI / (2.0f * (numSlices * 1.0f)))
	{
		step = 0;
		for (float horizontalAngle = 0; horizontalAngle < M_PI * 2; horizontalAngle += M_PI * 2.0f / (numSegments *
			     1.0f))
		{
			DirectX::XMFLOAT4 vertex(1.0f, 0.0f, 0.0f, 1.0f);
			vertex.x = cos(horizontalAngle) * r * cos(verticalAngle);
			vertex.y = sin(horizontalAngle) * r * cos(verticalAngle);
			vertex.z = r * sin(verticalAngle);
			vertices.push_back(vertex);

			DirectX::XMFLOAT3 tmp(vertex.x, vertex.y, vertex.z);
			DirectX::XMVECTOR vvertex = XMLoadFloat3(&tmp);
			vvertex = DirectX::XMVector3Normalize(vvertex);
			XMStoreFloat4(&vertex, vvertex);
			vertex.x = atan2(vertex.x, vertex.z) / (2 * M_PI) + 0.5f;
			vertex.y = vertex.y * 0.5f + 0.5f;

			vertices.push_back(vertex);
			step++;
		}
	}

	vertices.push_back(DirectX::XMFLOAT4(0, 0, -r, 1.0f));
	DirectX::XMFLOAT4 tmp(0, 0, -1, 1.0f);
	tmp.x = atan2(0, -1) / (2 * M_PI) + 0.5f;
	tmp.y = 0;

	vertices.push_back(tmp);

	for (int startIndex = 0; startIndex < step - 1; startIndex++)
	{
		indices.push_back(startIndex);
		indices.push_back(vertices.size() / 2 - 1);
		indices.push_back(startIndex + 1);
	}
	indices.push_back(step - 1);
	indices.push_back(vertices.size() / 2 - 1);
	indices.push_back(0);

	for (int startIndex = 0; startIndex < step * (numSlices - 1) * 2; startIndex += step)
	{
		for (int i = startIndex; i < startIndex + step - 1; i++)
		{
			indices.push_back(i);
			indices.push_back(i + 1);
			indices.push_back(i + step);

			indices.push_back(i + 1);
			indices.push_back(i + 1 + step);
			indices.push_back(i + step);
		}

		indices.push_back(startIndex + step - 1);
		indices.push_back(startIndex + step);
		indices.push_back(startIndex + step * 2 - 1);

		indices.push_back(startIndex);
		indices.push_back(startIndex + step);
		indices.push_back(startIndex + step - 1);
	}

	vertices.push_back(DirectX::XMFLOAT4(0, 0, r, 1.0f));

	tmp.x = atan2(0, 1) / (2 * M_PI) + 0.5f;
	tmp.y = 0;
	vertices.push_back(tmp);

	for (int startIndex = step * (numSlices - 1) * 2; startIndex < step * (numSlices - 1) * 2 + step - 1; startIndex++)
	{
		indices.push_back(startIndex);
		indices.push_back(startIndex + 1);
		indices.push_back(vertices.size() / 2 - 1);
	}
	indices.push_back(step * (numSlices - 1) * 2 + step - 1);
	indices.push_back(step * (numSlices - 1) * 2);

	indices.push_back(vertices.size() / 2 - 1);
}

void SphereObject::Init()
{
	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	game->device->CreateSamplerState(&sampDesc, &this->samplerState);

	ID3DBlob* vertexBC;
	ID3DBlob* pixelBC;

	D3D_SHADER_MACRO Shader_Macros[] = {"TEST", "1", "TCOLOR", "float4(0.0f, 1.0f, 0.0f, 1.0f)", nullptr, nullptr};

	D3DCompileFromFile(shaderPath,
	                   nullptr /*macros*/,
	                   nullptr /*include*/,
	                   "VSMain",
	                   "vs_5_0",
	                   D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
	                   0,
	                   &vertexBC,
	                   nullptr);

	D3DCompileFromFile(shaderPath,
	                   Shader_Macros /*macros*/,
	                   nullptr /*include*/,
	                   "PSMain",
	                   "ps_5_0",
	                   D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
	                   0,
	                   &pixelBC,
	                   nullptr);

	game->device->CreateVertexShader(
		vertexBC->GetBufferPointer(),
		vertexBC->GetBufferSize(),
		nullptr, pVertexShader.GetAddressOf());

	game->device->CreatePixelShader(
		pixelBC->GetBufferPointer(),
		pixelBC->GetBufferSize(),
		nullptr, pPixelShader.GetAddressOf());

	const D3D11_INPUT_ELEMENT_DESC inputElements[] = {
		D3D11_INPUT_ELEMENT_DESC{
			"POSITION",
			0,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			0,
			0,
			D3D11_INPUT_PER_VERTEX_DATA,
			0
		},
		D3D11_INPUT_ELEMENT_DESC{
			"COLOR",
			0,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			0,
			D3D11_APPEND_ALIGNED_ELEMENT,
			D3D11_INPUT_PER_VERTEX_DATA,
			0
		}
	};

	game->device->CreateInputLayout(
		inputElements,
		2,
		vertexBC->GetBufferPointer(),
		vertexBC->GetBufferSize(),
		pInputLayout.GetAddressOf());

	GenerateSphere(gc->r, 64, 64);

	D3D11_BUFFER_DESC bd;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.CPUAccessFlags = 0u;
	bd.MiscFlags = 0u;
	bd.ByteWidth = sizeof(DirectX::XMFLOAT4) * vertices.size();
	bd.StructureByteStride = 0u;

	D3D11_SUBRESOURCE_DATA sd = {};
	sd.pSysMem = static_cast<void*>(vertices.data());
	sd.SysMemPitch = 0;
	sd.SysMemSlicePitch = 0;

	game->device->CreateBuffer(&bd, &sd, &pVertexBuffer);

	strides = new UINT[1]{32u};
	offsets = new UINT[1]{0u};

	D3D11_BUFFER_DESC ibd = {};
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.Usage = D3D11_USAGE_DEFAULT;
	ibd.CPUAccessFlags = 0u;
	ibd.MiscFlags = 0u;
	ibd.ByteWidth = indices.size() * sizeof(int);
	ibd.StructureByteStride = 0u;

	D3D11_SUBRESOURCE_DATA isd = {};
	isd.pSysMem = static_cast<void*>(indices.data());
	isd.SysMemPitch = 0;
	isd.SysMemSlicePitch = 0;

	game->device->CreateBuffer(&ibd, &isd, &pIndexBuffer);

	CD3D11_BUFFER_DESC cbd;
	cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbd.Usage = D3D11_USAGE_DYNAMIC;
	cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbd.MiscFlags = 0u;
	cbd.ByteWidth = sizeof(CB_VS_vertexshader) + (16 - sizeof(CB_VS_vertexshader)) % 16;
	cbd.StructureByteStride = 0u;

	game->device->CreateBuffer(&cbd, nullptr, pConstantBuffer.GetAddressOf());

	DirectX::CreateWICTextureFromFile(game->device, filepath, nullptr, &myTexture);

	CD3D11_RASTERIZER_DESC rastDesc = {};
	rastDesc.CullMode = D3D11_CULL_NONE;
	rastDesc.FillMode = D3D11_FILL_SOLID;

	ID3D11RasterizerState* rastState;

	game->device->CreateRasterizerState(&rastDesc, &rastState);

	game->context->RSSetState(rastState);

	D3D11_VIEWPORT viewport = {};
	viewport.Width = static_cast<float>(game->display->initClientWidth);
	viewport.Height = static_cast<float>(game->display->initClientHeight);
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.MinDepth = 0;
	viewport.MaxDepth = 1.0f;

	game->context->RSSetViewports(1, &viewport);
}

void SphereObject::Draw()
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;

	game->context->Map(pConstantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	CopyMemory(mappedResource.pData, &gc->data, sizeof(CB_VS_vertexshader));
	game->context->Unmap(pConstantBuffer.Get(), 0);
	game->context->VSSetConstantBuffers(0, 1, pConstantBuffer.GetAddressOf());

	game->context->IASetInputLayout(pInputLayout.Get());
	game->context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	game->context->IASetIndexBuffer(pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	game->context->IASetVertexBuffers(0, 1, &pVertexBuffer, strides, offsets);
	game->context->PSSetSamplers(0, 1, &this->samplerState);
	game->context->VSSetShader(pVertexShader.Get(), nullptr, 0);
	game->context->PSSetShader(pPixelShader.Get(), nullptr, 0);
	game->context->PSSetShaderResources(0, 1, &this->myTexture);
	game->context->OMSetRenderTargets(1, &game->renderTargetView, game->depthStencilView);

	game->context->DrawIndexed(indices.size(), 0, 0);

	game->context->OMSetRenderTargets(0, nullptr, nullptr);
}
