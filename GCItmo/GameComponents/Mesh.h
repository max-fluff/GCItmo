#pragma once
#include <d3dcommon.h>
#include <DirectXMath.h>
#include <vector>
#include <wrl/client.h>

#include <d3d11.h>

#include "../GameComponent.h"

#include <vector>

class Game;

class Mesh : public GameComponent
{
public:
	Mesh(Game* _game, std::vector<DirectX::XMFLOAT4>& vertices, std::vector<int>& indices, LPCWSTR _filepath);
	void Draw() override;
	void Init() override;
	DirectX::XMMATRIX transformMatrix;

	std::vector< DirectX::XMFLOAT4 > verts;
	std::vector< int > index;
	UINT strides[1] = { 48 };
	UINT offsets[1] = { 0 };
	Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerState;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> myTexture;

	LPCWSTR filepath;
private:
	ID3D11Buffer* pIndexBuffer;
	ID3D11Buffer* pVertexBuffer;
	int indicesSize;

	ID3D11Buffer* constantBuffer;
	Game* game;

	LPCWSTR shaderPath = L"./Shaders/ModelShader.hlsl";

	Microsoft::WRL::ComPtr<ID3D11InputLayout> layout;
	Microsoft::WRL::ComPtr<ID3DBlob> vertexBC;
	Microsoft::WRL::ComPtr<ID3DBlob> pixelBC;

	Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> pixelShader;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> rastState;
};
