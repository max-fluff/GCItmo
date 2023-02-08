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
	static DisplayWin32& display;
	IDXGISwapChain* swapChain;
	ID3D11DeviceContext* context;
	ID3D11VertexShader* vertexShader;
	ID3D11PixelShader* pixelShader;
	ID3D11InputLayout* layout;
	ID3D11Buffer* indexBuffer;
	ID3D11Buffer* vertexBuffer;
	ID3D11RenderTargetView* renderTargetView;
	ID3D11RasterizerState* rastState;

	UINT* strides;
	UINT* offsets;

	float totalTime;

	Game();

	void Run();
public:
	static Game& GetInstance();

	void Initialize();
	void Update(float deltaTime);
};
