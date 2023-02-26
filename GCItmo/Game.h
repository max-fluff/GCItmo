#pragma once
//#include "DisplayWin32.h"
#include "GameComponent.h"
#include "InputDevice.h"
#include <d3d11.h>
#include <vector>
#include <wrl/client.h>

#include "Physics2DObject.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")

class DisplayWin32;

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
	std::vector<std::pair<Physics2DObject*, Physics2DObject*>> collidedPair;

	int test = 2;

	Game();

	void SetSwapDesc();
	void CreateDeviceAndSwapChain();
	void PreDraw();
	void PostDraw();

	void Run();
public:
	Microsoft::WRL::ComPtr<ID3D11Device> device;
	ID3D11DeviceContext* context;
	InputDevice* inputDevice;

	static Game& GetInstance();

	void Initialize();

	void Update(float deltaTime);
	void ProcessCollisions();
};
