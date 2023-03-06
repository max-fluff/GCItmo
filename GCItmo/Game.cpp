#include "Game.h"
#include <iostream>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <chrono>

#include "Camera.h"
#include "DisplayWin32.h"
#include "GameComponents/Planet.h"

Game* Game::instance = nullptr;

Game& Game::GetInstance()
{
	if (instance == nullptr)
		instance = new Game();

	return *instance;
}

void Game::CreateDeviceAndSwapChain()
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
}

void Game::Initialize()
{
	constexpr auto winHeight = 800;
	constexpr auto winWidth = 1200;

	display = new DisplayWin32(winHeight, winWidth, L"My3D App", this);
	wInput = new WinInput(this);

	CreateDeviceAndSwapChain();

	camera = new Camera();
	camera->SetPosition(-5.0f, 0.0f, 0.0f);
	camera->SetRotation(90.0f, 0, 0);
	camera->SetProjectionValues(90.0f, (float)winWidth / (float)winHeight, 0.1f, 1000.0f);

	cameraController = new CameraController(camera, this);

	D3D11_TEXTURE2D_DESC textureDesc;
	textureDesc.Width = winWidth;
	textureDesc.Height = winHeight;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	this->device->CreateTexture2D(&textureDesc, nullptr, &depthStencilBuffer);

	this->device->CreateDepthStencilView(depthStencilBuffer, nullptr, &depthStencilView);

	context->OMSetRenderTargets(1, &renderTargetView, depthStencilView);

	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));

	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

	device->CreateDepthStencilState(&depthStencilDesc, &depthStencilState);

	D3D11_VIEWPORT viewport;
	viewport.Width = static_cast<float>(winWidth);
	viewport.Height = static_cast<float>(winHeight);
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.MinDepth = 0;
	viewport.MaxDepth = 1.0f;

	context->RSSetViewports(1, &viewport);

	auto sun = new Planet(this, 0.5f, 0.2f, 0.0f, 0.0f, nullptr);
	auto mercury = new Planet(this, 0.1f, 0.5f, 1.0f, 1.5f, nullptr);
	auto mercury1 = new Planet(this, 0.03f, 10.0f, 0.2f, 0.2f, mercury);
	auto venus = new Planet(this, 0.2f, 2.5f, 1.1f, 3.0f, nullptr);
	auto earth = new Planet(this, 0.15f, 2.0f, 1.0f, 4.5f, nullptr);
	auto moon = new Planet(this, 0.02f, 2.0f, 1.0f, 0.5f, earth);
	auto mars = new Planet(this, 0.15f, 1.9f, 0.8f, 5.5f, nullptr);
	auto jupiter = new Planet(this, 0.4f, 1.6f, 0.6f, 6.5f, nullptr);
	auto saturn = new Planet(this, 0.35f, 1.4f, 0.5f, 7.5f, nullptr);
	auto uranus = new Planet(this, 0.3f, 1.0f, 0.4f, 9.0f, nullptr);
	auto neptune = new Planet(this, 0.35f, 0.0f, 0.3f, 10.0f, nullptr);
	auto pluto = new Planet(this, 0.05f, 1.7f, 0.1f, 11.0f, nullptr);

	components.push_back(new SphereObject(this, sun, L"Textures\\obama.png"));
	components.push_back(new SphereObject(this, mercury, L"Textures\\obama.png"));
	components.push_back(new SphereObject(this, mercury1, L"Textures\\obama.png"));
	components.push_back(new SphereObject(this, venus, L"Textures\\obama.png"));
	components.push_back(new SphereObject(this, earth, L"Textures\\obama.png"));
	components.push_back(new SphereObject(this, moon, L"Textures\\obama.png"));
	components.push_back(new SphereObject(this, mars, L"Textures\\obama.png"));
	components.push_back(new SphereObject(this, jupiter, L"Textures\\obama.png"));
	components.push_back(new SphereObject(this, saturn, L"Textures\\obama.png"));
	components.push_back(new SphereObject(this, uranus, L"Textures\\obama.png"));
	components.push_back(new SphereObject(this, neptune, L"Textures\\obama.png"));
	components.push_back(new SphereObject(this, pluto, L"Textures\\obama.png"));
	components.push_back(sun);
	components.push_back(mercury);
	components.push_back(mercury1);
	components.push_back(venus);
	components.push_back(earth);
	components.push_back(moon);
	components.push_back(mars);
	components.push_back(jupiter);
	components.push_back(saturn);
	components.push_back(uranus);
	components.push_back(neptune);
	components.push_back(pluto);

	for (const auto component : components)
		component->Init();
}

void Game::Update(float deltaTime)
{
	totalTime += deltaTime;

	if (totalTime > 1.0f)
		totalTime = 0;

	wInput->GetInput();

	for (const auto component : components)
		component->Update(deltaTime);

	cameraController->CameraMovement(deltaTime);

	PreDraw();

	for (const auto component : components)
		component->Draw();

	PostDraw();
}

void Game::PreDraw()
{
	constexpr float bgColor[] = {0.4f, 0.2f, 0.8f, 1.0f};
	context->ClearRenderTargetView(renderTargetView, bgColor);
	context->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	//context->OMSetRenderTargets(1, &renderTargetView, depthStencilView);

	const DirectX::XMMATRIX world = DirectX::XMMatrixIdentity();
	worldViewData.worldViewProj = world * camera->GetViewMatrix() * camera->GetProjectionMatrix();
}

void Game::PostDraw() const
{
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

	while (wInput->ProcessMessages())
	{
		auto curTime = std::chrono::steady_clock::now();
		const float deltaTime = std::chrono::duration_cast<std::chrono::microseconds>(curTime - PrevTime).count() /
			1000000.0f;
		PrevTime = curTime;
		Update(deltaTime);
	}
}
