#pragma once
#include <windows.h>
#include <WinUser.h>
#include <wrl.h>
#include <iostream>
#include <d3d.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <chrono>
#include <vector>
#include <memory>


#include "DisplayWin32.h"
#include "Input/WinInput.h"

#include "RenderComponents/GameComponent.h"

#include "Camera/Camera.h"
#include "Camera/CameraController.h"

#include "MyGame/Components/MyDebugPlane.h"
#include "MyGame/Models/Model3D.h"
#include "MyGame/Components/MyCube.h"
#include "Graphics/DirectionalLight.h"
#include "ConstantBufferTypes.h"
#include "Graphics/RenderTexture.h"
#include "Graphics/Shadows/DebugWindowClass.h"
#include "Graphics/Shadows/RenderTextureClass.h"
#include "Graphics/Shadows/TextureShaderClass.h"


#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")


class GameComponent;


class Game
{
public:


	Game(LPCWSTR applicationName, HINSTANCE hInstance, int width, int height);

	~Game();

	HWND GetWindow32HWND();

	void ShadowRender();

	void CreateDeviceAndSwapChain();

	void InitializeDirectX();

	void StartGameLoop();

	void AddGameComponent(std::shared_ptr<GameComponent> gc);

	std::unique_ptr<DisplayWin32> window;
	std::unique_ptr<WinInput> wInput;


	Camera* camera;
	CameraController* cameraController;
	
	DXGI_SWAP_CHAIN_DESC swapDesc;
	Microsoft::WRL::ComPtr<ID3D11Device> device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> context;
	Microsoft::WRL::ComPtr <IDXGISwapChain> swapChain;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> rtv;
	Microsoft::WRL::ComPtr < ID3D11Texture2D> backTex;
	Microsoft::WRL::ComPtr < ID3D11RasterizerState> rastState;
	Microsoft::WRL::ComPtr <ID3D11DepthStencilView> depthStencilView;
	Microsoft::WRL::ComPtr <ID3D11Texture2D> depthStencilBuffer;
	Microsoft::WRL::ComPtr <ID3D11DepthStencilView> shadowStencilView;

	Microsoft::WRL::ComPtr <ID3D11DepthStencilState> depthStencilState;
	Microsoft::WRL::ComPtr <ID3D11DepthStencilState> shadowStencilState;
	ID3D11Texture2D* texture_ ;

	RenderTextureClass* renderTexture;
	DebugWindowClass* debugWindow;
	TextureShaderClass* textureShader;
	ID3D11RenderTargetView* shadowTargetView;
	ID3D11ShaderResourceView* shadowResourceView;
	
	CB_VS_vertexshader worldViewData;
	
	bool isExitRequested = false;

	
	std::vector<std::shared_ptr<GameComponent>> gameComponents;
	
	float deltaTime = 0;
	std::shared_ptr<DirectionalLight> dirLight;

	const float SHADOWMAP_WIDTH =2048;
	const float SHADOWMAP_HEIGHT=2048;


private:
	void GetInput();
	void Render();
	void Update();
	

	std::chrono::time_point<std::chrono::steady_clock> PrevTime;
	float totalTime = 0;
	
	unsigned int frameCount = 0;
};

