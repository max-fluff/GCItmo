#include "RectObject.h"
#include <d3dcompiler.h>
#include <iostream>

#include "Game.h"

struct ConstantBuffer
{
	DirectX::XMMATRIX transform;
};

RectObject::RectObject(Game* game, Vertex* vertex, float width, float height)
	: game(game), startVertex(vertex), width(width), height(height)
{
	Vertex vertices[] =
	{
		{
			vertex->pos.x + width / 2, vertex->pos.y + height / 2, vertex->color.r, vertex->color.g, vertex->color.b,
			vertex->color.a
		},
		{
			vertex->pos.x - width / 2, vertex->pos.y - height / 2, vertex->color.r, vertex->color.g, vertex->color.b,
			vertex->color.a
		},
		{
			vertex->pos.x + width / 2, vertex->pos.y - height / 2, vertex->color.r, vertex->color.g, vertex->color.b,
			vertex->color.a
		},
		{
			vertex->pos.x - width / 2, vertex->pos.y + height / 2, vertex->color.r, vertex->color.g, vertex->color.b,
			vertex->color.a
		}
	};

	D3D11_BUFFER_DESC bd;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.CPUAccessFlags = 0u;
	bd.MiscFlags = 0u;
	bd.ByteWidth = sizeof vertices;
	bd.StructureByteStride = sizeof(Vertex);

	D3D11_SUBRESOURCE_DATA sd = {};
	sd.pSysMem = vertices;

	game->device->CreateBuffer(
		&bd,
		&sd,
		&pVertexBuffer
	);

	const unsigned short indices[] =
	{
		0, 2, 1,
		0, 1, 3
	};


	const UINT stride = sizeof(Vertex);
	const UINT offset = 0u;
	game->context->IASetVertexBuffers(
		0u,
		1u,
		&pVertexBuffer,
		&stride,
		&offset
	);

	D3D11_BUFFER_DESC ibd = {};
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.Usage = D3D11_USAGE_DEFAULT;
	ibd.CPUAccessFlags = 0u;
	ibd.MiscFlags = 0u;
	ibd.ByteWidth = sizeof(indices);
	ibd.StructureByteStride = sizeof(unsigned short);
	D3D11_SUBRESOURCE_DATA isd = {};
	isd.pSysMem = indices;
	game->device->CreateBuffer(&ibd, &isd, &pIndexBuffer);

	game->context->IASetIndexBuffer(pIndexBuffer, DXGI_FORMAT_R16_UINT, 0u);

	ComPtr<ID3DBlob> pBlob;
	D3DReadFileToBlob(L"Shaders/PixelShader.cso", &pBlob);
	game->device->CreatePixelShader(
		pBlob->GetBufferPointer(),
		pBlob->GetBufferSize(),
		nullptr,
		&pPixelShader
	);

	game->context->PSSetShader(pPixelShader.Get(), nullptr, 0u);

	D3DReadFileToBlob(L"Shaders/VertexShader.cso", &pBlob);
	game->device->CreateVertexShader(
		pBlob->GetBufferPointer(),
		pBlob->GetBufferSize(),
		nullptr,
		&pVertexShader
	);

	game->context->VSSetShader(pVertexShader.Get(), nullptr, 0u);

	const D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{"Position", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"Color", 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, 8u, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	game->device->CreateInputLayout(
		ied,
		(UINT)std::size(ied),
		pBlob->GetBufferPointer(),
		pBlob->GetBufferSize(),
		&pInputLayout
	);

	CD3D11_BUFFER_DESC cbd;
	cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbd.Usage = D3D11_USAGE_DEFAULT;
	cbd.CPUAccessFlags = 0u;
	cbd.MiscFlags = 0u;
	cbd.ByteWidth = sizeof(ConstantBuffer);
	cbd.StructureByteStride = 0u;
	game->device->CreateBuffer(&cbd, nullptr, &pConstantBuffer);

	game->context->VSSetConstantBuffers(0u, 1u, pConstantBuffer.GetAddressOf());
}

void RectObject::Draw()
{
	const UINT stride = sizeof(Vertex);
	const UINT offset = 0u;
	game->context->IASetVertexBuffers(
		0u,
		1u,
		&pVertexBuffer,
		&stride,
		&offset
	);

	game->context->IASetIndexBuffer(pIndexBuffer, DXGI_FORMAT_R16_UINT, 0u);

	game->context->PSSetShader(pPixelShader.Get(), nullptr, 0u);

	game->context->VSSetShader(pVertexShader.Get(), nullptr, 0u);

	game->context->IASetInputLayout(pInputLayout.Get());

	game->context->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	const unsigned short indices[] =
	{
		0, 2, 1,
		0, 1, 3
	};

	const ConstantBuffer cb =
	{
		{
			XMMatrixTranspose(
				DirectX::XMMatrixTranslation(
					localPosX,
					localPosY,
					0)
			)
		}
	};
	game->context->VSSetConstantBuffers(0u, 1u, pConstantBuffer.GetAddressOf());
	game->context->UpdateSubresource(pConstantBuffer.Get(), 0, NULL, &cb, 0, 0);

	game->context->DrawIndexed((UINT)std::size(indices), 0u, 0u);
}

Pos RectObject::GetPosition()
{
	return {GetPositionX(), GetPositionY()};
}

float RectObject::GetPositionX()
{
	return startVertex->pos.x + localPosX;
}

float RectObject::GetPositionY()
{
	return startVertex->pos.y + localPosY;
}

void RectObject::SetPosition(Pos pos)
{
	localPosX = pos.x - startVertex->pos.x;
	localPosY = pos.y - startVertex->pos.y;
}

void RectObject::SetPositionX(float posX)
{
	localPosX = posX - startVertex->pos.x;
}

void RectObject::SetPositionY(float posY)
{
	localPosY = posY - startVertex->pos.y;
}

float RectObject::GetWidth()
{
	return width;
}

float RectObject::GetHeight()
{
	return height;
}

Pos RectObject::GetLeftTop()
{
	return {GetPositionX() - width / 2, GetPositionY() + height / 2};
}

Pos RectObject::GetRightTop()
{
	return {GetPositionX() + width / 2, GetPositionY() + height / 2};
}

Pos RectObject::GetLeftBottom()
{
	return {GetPositionX() - width / 2, GetPositionY() - height / 2};
}

Pos RectObject::GetRightBottom()
{
	return {GetPositionX() + width / 2, GetPositionY() - height / 2};
}
