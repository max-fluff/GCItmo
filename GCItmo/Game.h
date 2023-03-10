#pragma once
#include "DisplayWin32.h"
#include "GameComponent.h"
#include <d3d11.h>
#include <vector>

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
	DXGI_SWAP_CHAIN_DESC swapDesc;
	ID3D11Texture2D* backTex;
	ID3D11RenderTargetView* renderTargetView;
	ID3D11RasterizerState* rastState;
	CD3D11_RASTERIZER_DESC rastDesc = {};

	float totalTime;

	std::vector<GameComponent*> components;

	Game();

	void SetSwapDesc();
	void CreateDeviceAndSwapChain();
	void Render();
	void PreDraw();
	void PostDraw();

	void Run();
public:
	Microsoft::WRL::ComPtr<ID3D11Device> device;
	ID3D11DeviceContext* context;

	static Game& GetInstance();

	void Initialize();

	void Update(float deltaTime);
};
