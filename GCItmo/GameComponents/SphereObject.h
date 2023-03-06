#pragma once
#include <WTypesbase.h>
#include <winnt.h>
#include <vector>
#include <directxmath.h>
#include <windows.h>
#include <wrl/client.h>

#include <d3d11.h>

#include "../GameComponent.h"

class Planet;
class Game;

class SphereObject: public GameComponent
{
	const wchar_t* filepath;
	LPCWSTR shaderPath = L"./Shaders/SphereObject.hlsl";
	Planet* gc;
	Game* game;
	std::vector<DirectX::XMFLOAT4> vertices;
	std::vector<int> indices;
	ID3D11SamplerState* samplerState;

	ID3D11Buffer* pIndexBuffer;
	ID3D11Buffer* pVertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> pInputLayout;
	Microsoft::WRL::ComPtr<ID3D11VertexShader> pVertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> pPixelShader;
	Microsoft::WRL::ComPtr<ID3D11Buffer> pConstantBuffer;

	UINT* strides;
	UINT* offsets;

	ID3D11ShaderResourceView* myTexture;

public:
	SphereObject(Game* game, Planet* gc, LPCWSTR filepath);
	void GenerateSphere(float r, int segments, int slices);
	void Init() override;
	void Draw() override;
};
