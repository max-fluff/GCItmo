#include "Game.h"
#include <iostream>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <chrono>

#include "FirstLessonSquare.h"
#include "Player.h"
#include "RectObject.h"

Game* Game::instance = nullptr;

Game& Game::GetInstance()
{
	if (instance == nullptr)
		instance = new Game();

	return *instance;
}

void Game::SetSwapDesc()
{
	swapDesc = {};
	swapDesc.BufferCount = 2;
	swapDesc.BufferDesc.Width = display->initClientWidth;
	swapDesc.BufferDesc.Height = display->initClientHeight;
	swapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapDesc.OutputWindow = display->hWnd;
	swapDesc.Windowed = true;
	swapDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	swapDesc.SampleDesc.Count = 1;
	swapDesc.SampleDesc.Quality = 0;
}

void Game::CreateDeviceAndSwapChain()
{
	constexpr D3D_FEATURE_LEVEL featureLevel[] = {D3D_FEATURE_LEVEL_11_1};

	D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		D3D11_CREATE_DEVICE_DEBUG,
		featureLevel,
		1,
		D3D11_SDK_VERSION,
		&swapDesc,
		&swapChain,
		&device,
		nullptr,
		&context);


	swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backTex);
	device->CreateRenderTargetView(backTex, nullptr, &renderTargetView);

	rastDesc.CullMode = D3D11_CULL_NONE;
	rastDesc.FillMode = D3D11_FILL_SOLID;

	device->CreateRasterizerState(&rastDesc, &rastState);

	context->RSSetState(rastState);
}

void Game::Initialize()
{
	constexpr auto winHeight = 800;
	constexpr auto winWidth = 1200;

	display = new DisplayWin32(winHeight, winWidth, L"My3D App");

	inputDevice = new InputDevice();

	SetSwapDesc();
	CreateDeviceAndSwapChain();

	const auto leftPlayerBarVertex = new Vertex{-0.5f + 0.1f, 0.0f, 255, 0, 0, 1};
	const auto rightPlayerBarVertex = new Vertex{0.5f - 0.1f, 0.0f, 255, 0, 0, 1};

	constexpr auto playerHeight = 1.0f;

	const auto leftPlayerRect = new RectObject(this, leftPlayerBarVertex, 0.1f, playerHeight);
	const auto rightPlayerRect = new RectObject(this, rightPlayerBarVertex, 0.1f, playerHeight);

	components.push_back(new Player(inputDevice, leftPlayerRect, 'S', 'W', playerHeight / 2.0f - 0.5f, 0.5f - playerHeight / 2.0f));
	components.push_back(new Player(inputDevice, rightPlayerRect, VK_DOWN, VK_UP, playerHeight / 2.0f - 0.5f, 0.5f - playerHeight / 2.0f));
	components.push_back(leftPlayerRect);
	components.push_back(rightPlayerRect);
	components.push_back(new RectObject(this, new Vertex{0.0f, 0.0f, 255, 255, 255, 1}, 30.0f / winWidth,
	                                    30.0f / winHeight));

	for (const auto component : components)
		component->Init();
}

void Game::Update(float deltaTime)
{
	totalTime += deltaTime;

	if (totalTime > 1.0f)
		totalTime = 0;

	for (auto component : components)
		component->Update(deltaTime);

	PreDraw();

	for (auto component : components)
		component->Draw();

	PostDraw();
}

void Game::PreDraw()
{
	context->ClearState();

	context->RSSetState(rastState);

	D3D11_VIEWPORT viewport = {};
	viewport.Width = static_cast<float>(display->initClientWidth);
	viewport.Height = static_cast<float>(display->initClientHeight);
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.MinDepth = 0;
	viewport.MaxDepth = 1.0f;

	context->RSSetViewports(1, &viewport);

	context->OMSetRenderTargets(1, &renderTargetView, nullptr);

	constexpr float color[] = {0.1f, 0.1f, 0.1f, 1.0f};
	context->ClearRenderTargetView(renderTargetView, color);
}

void Game::PostDraw()
{
	context->OMSetRenderTargets(0, nullptr, nullptr);

	swapChain->Present(1, 0);
}

Game::Game()
{
	Initialize();
	Run();
}

void Game::Run()
{
	std::chrono::time_point<std::chrono::steady_clock> PrevTime = std::chrono::steady_clock::now();
	totalTime = 0;

	MSG msg = {};
	bool isExitRequested = false;

	while (!isExitRequested)
	{
		while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (msg.message == WM_QUIT)
		{
			isExitRequested = true;
		}

		auto curTime = std::chrono::steady_clock::now();
		const float deltaTime = std::chrono::duration_cast<std::chrono::microseconds>(curTime - PrevTime).count() /
			1000000.0f;
		PrevTime = curTime;

		Update(deltaTime);
	}
}
