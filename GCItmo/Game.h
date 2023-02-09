#pragma once
#include "DisplayWin32.h"
#include <d3d.h>
#include <d3d11.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")

class Game
{
private:
	static Game* instance;
	DisplayWin32* display;
	IDXGISwapChain* swapChain;
	ID3D11DeviceContext* context;
	DXGI_SWAP_CHAIN_DESC swapDesc;
	ID3D11Texture2D* backTex;
	Microsoft::WRL::ComPtr<ID3D11Device> device;
	ID3D11VertexShader* vertexShader;
	ID3D11PixelShader* pixelShader;
	ID3D11InputLayout* layout;
	ID3D11Buffer* indexBuffer;
	ID3D11Buffer* vertexBuffer;
	ID3D11RenderTargetView* renderTargetView;
	ID3D11RasterizerState* rastState;

	ID3DBlob* vertexBC;
	ID3DBlob* errorVertexCode;
	CD3D11_RASTERIZER_DESC rastDesc = {};
	LPCWSTR fileName = L"Shaders/MyVeryFirstShader.hlsl";

	UINT strides[1] = { 32 }; //m
	UINT offsets[1] = { 0 }; //m

	float totalTime;

	Game();

	void SetSwapDesc();
	void CreateDeviceAndSwapChain();
	void CompileFromFile();
	void Render();

	void Run();
public:
	static Game& GetInstance();

	void Initialize();

	void Update(float deltaTime);
};
