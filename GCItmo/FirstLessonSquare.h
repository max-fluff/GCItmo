#pragma once
#include <d3dcommon.h>
#include <d3dcompiler.h>
#include <directxmath.h>

#include "Game.h"
#include "GameComponent.h"

class FirstLessonSquare : public GameComponent
{
	LPCWSTR fileName = L"Shaders/MyVeryFirstShader.hlsl";
	Game game;
	ID3D11VertexShader* vertexShader;
	ID3D11PixelShader* pixelShader;
	ID3DBlob* vertexBC = nullptr;
	ID3DBlob* pixelBC = nullptr;
	ID3D11Buffer* indexBuffer;
	ID3D11Buffer* vertexBuffer;
	ID3D11InputLayout* layout;

	UINT strides[1] = {32};
	UINT offsets[1] = {0};

	int indexes[6] = {0, 1, 2, 1, 0, 3};

public:
	FirstLessonSquare(Game* game);
private:
	void Init() override;
	void Draw() override;
	void CompileShadersFromFile();
	void InitBuffers();
};

inline FirstLessonSquare::FirstLessonSquare(Game* game): game(*game)
{
}

inline void FirstLessonSquare::Init()
{
	CompileShadersFromFile();
	InitBuffers();
}

inline void FirstLessonSquare::Draw()
{
	game.context->IASetInputLayout(layout);
	game.context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	game.context->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	game.context->IASetVertexBuffers(0, 1, &vertexBuffer, strides, offsets);
	game.context->VSSetShader(vertexShader, nullptr, 0);
	game.context->PSSetShader(pixelShader, nullptr, 0);

	game.context->DrawIndexed(6, 0, 0);
}
 
inline void FirstLessonSquare::CompileShadersFromFile()
{
	D3DCompileFromFile(fileName,
	                   nullptr /*macros*/,
	                   nullptr /*include*/,
	                   "VSMain",
	                   "vs_5_0",
	                   D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
	                   0,
	                   &vertexBC,
	                   nullptr);

	D3D_SHADER_MACRO Shader_Macros[] = {"TEST", "1", "TCOLOR", "float4(0.0f, 1.0f, 0.0f, 1.0f)", nullptr, nullptr};

	D3DCompileFromFile(fileName,
	                   Shader_Macros /*macros*/,
	                   nullptr /*include*/,
	                   "PSMain",
	                   "ps_5_0",
	                   D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
	                   0,
	                   &pixelBC,
	                   nullptr);

	game.device->CreateVertexShader(
		vertexBC->GetBufferPointer(),
		vertexBC->GetBufferSize(),
		nullptr, &vertexShader);

	game.device->CreatePixelShader(
		pixelBC->GetBufferPointer(),
		pixelBC->GetBufferSize(),
		nullptr, &pixelShader);
}

inline void FirstLessonSquare::InitBuffers()
{
	D3D11_INPUT_ELEMENT_DESC inputElements[] = {
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

	game.device->CreateInputLayout(
		inputElements,
		2,
		vertexBC->GetBufferPointer(),
		vertexBC->GetBufferSize(),
		&layout);

	DirectX::XMFLOAT4 points[8] = {
		DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f), DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f),
		DirectX::XMFLOAT4(-0.5f, -0.5f, 0.5f, 1.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f),
		DirectX::XMFLOAT4(0.5f, -0.5f, 0.5f, 1.0f), DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f),
		DirectX::XMFLOAT4(-0.5f, 0.5f, 0.5f, 1.0f), DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
	};


	D3D11_BUFFER_DESC vertexBufDesc = {};
	vertexBufDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufDesc.CPUAccessFlags = 0;
	vertexBufDesc.MiscFlags = 0;
	vertexBufDesc.StructureByteStride = 0;
	vertexBufDesc.ByteWidth = sizeof(DirectX::XMFLOAT4) * std::size(points);

	D3D11_SUBRESOURCE_DATA vertexData = {};
	vertexData.pSysMem = points;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	game.device->CreateBuffer(&vertexBufDesc, &vertexData, &vertexBuffer);

	D3D11_BUFFER_DESC indexBufDesc = {};
	indexBufDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufDesc.CPUAccessFlags = 0;
	indexBufDesc.MiscFlags = 0;
	indexBufDesc.StructureByteStride = 0;
	indexBufDesc.ByteWidth = sizeof(int) * std::size(indexes);

	D3D11_SUBRESOURCE_DATA indexData = {};
	indexData.pSysMem = indexes;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	game.device->CreateBuffer(&indexBufDesc, &indexData, &indexBuffer);
}
