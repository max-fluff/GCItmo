#pragma once
#include <d3d11.h>
#include <vector>
#include <wrl/client.h>

#include "Camera.h"
#include "CameraController.h"
#include "ConstantBufferTypes.h"
#include "GameComponent.h"
#include "InputDevice.h"

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
	ID3D11RasterizerState* rastState;
	CD3D11_RASTERIZER_DESC rastDesc = {};

	ID3D11Texture2D* depthStencilBuffer;

	float totalTime;

	std::vector<GameComponent*> components;

	int test = 2;

	Game();

	void CreateDeviceAndSwapChain();
	void PreDraw();
	void PostDraw() const;

	void Run();
public:
	Microsoft::WRL::ComPtr<ID3D11Device> device;
	ID3D11DeviceContext* context;
	InputDevice* inputDevice;
	Camera* camera;
	CameraController* cameraController;
	CB_VS_vertexshader worldViewData;
	ID3D11RenderTargetView* renderTargetView;
	ID3D11DepthStencilView* depthStencilView;
	ID3D11DepthStencilState* depthStencilState;

	static Game& GetInstance();

	void Initialize();

	void Update(float deltaTime);
};
