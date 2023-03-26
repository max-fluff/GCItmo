#include "Game.h"

#include "ScreenGrab.h"
#include "Graphics/Shadows/RenderTextureClass.h"
#include "MyGame/Components/Player.h"
#include "windows.h"
#include <ScreenGrab.h>


Game::Game(LPCWSTR applicationName, HINSTANCE hInstance, int width, int height)
{
	window = std::make_unique<DisplayWin32>(this, applicationName, hInstance, width, height);
	wInput = std::make_unique<WinInput>(this);
}

Game::~Game()
{
}


HWND Game::GetWindow32HWND()
{
	return window->GetHWND();
}

void Game::CreateDeviceAndSwapChain()
{
	swapDesc = {};
	swapDesc.BufferCount = 2;
	swapDesc.BufferDesc.Width = window->GetWidth();
	swapDesc.BufferDesc.Height = window->GetHeight();
	swapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapDesc.OutputWindow = window->GetHWND();
	swapDesc.Windowed = true;
	swapDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	swapDesc.SampleDesc.Count = 1;
	swapDesc.SampleDesc.Quality = 0;

	D3D_FEATURE_LEVEL featureLevel[] = {D3D_FEATURE_LEVEL_11_1};

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

	swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)backTex.GetAddressOf()); // __uuidof(ID3D11Texture2D)
	device->CreateRenderTargetView(backTex.Get(), nullptr, rtv.GetAddressOf());
}

void Game::InitializeDirectX()
{
	CreateDeviceAndSwapChain();

	camera = new Camera();
	camera->SetPosition(0.0f, 0.0f, 0.0f);
	camera->SetLookAtPos(DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));
	camera->SetProjectionValues(90.0f, window->GetWidth() / window->GetHeight(), 0.1f, 15.0f);

	renderTexture = new RenderTextureClass;
	renderTexture->Initialize(device.Get(), SHADOWMAP_WIDTH, SHADOWMAP_HEIGHT);

	debugWindow = new DebugWindowClass;
	debugWindow->Initialize(device.Get(), 800, 800, 200, 200);

	textureShader = new TextureShaderClass;
	textureShader->Initialize(device.Get(), window->GetHWND());

	D3D11_TEXTURE2D_DESC depthStencilDesc;
	depthStencilDesc.Width = window->GetWidth();
	depthStencilDesc.Height = window->GetHeight();
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.SampleDesc.Count = 1;
	depthStencilDesc.SampleDesc.Quality = 0;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;

	HRESULT hr = this->device->CreateTexture2D(&depthStencilDesc, NULL, this->depthStencilBuffer.GetAddressOf());
	hr = this->device->CreateDepthStencilView(this->depthStencilBuffer.Get(), NULL,
	                                          this->depthStencilView.GetAddressOf());


	context->OMSetRenderTargets(1, rtv.GetAddressOf(), this->depthStencilView.Get());

	D3D11_DEPTH_STENCIL_DESC depthstencildesc;
	ZeroMemory(&depthstencildesc, sizeof(D3D11_DEPTH_STENCIL_DESC));

	depthstencildesc.DepthEnable = true;
	depthstencildesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL;
	depthstencildesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL;

	hr = this->device->CreateDepthStencilState(&depthstencildesc, this->depthStencilState.GetAddressOf());


	D3D11_TEXTURE2D_DESC shadowMapDesc;
	ZeroMemory(&shadowMapDesc, sizeof(D3D11_TEXTURE2D_DESC));
	shadowMapDesc.Width = SHADOWMAP_WIDTH;
	shadowMapDesc.Height = SHADOWMAP_HEIGHT;
	shadowMapDesc.MipLevels = 1;
	shadowMapDesc.ArraySize = 1;
	shadowMapDesc.Format = DXGI_FORMAT_R32_TYPELESS;
	shadowMapDesc.SampleDesc.Count = 1;
	shadowMapDesc.SampleDesc.Quality = 0;
	shadowMapDesc.Usage = D3D11_USAGE_DEFAULT;
	shadowMapDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	shadowMapDesc.CPUAccessFlags = 0;
	shadowMapDesc.MiscFlags = 0;

	hr = device->CreateTexture2D(&shadowMapDesc, nullptr, &texture_);

	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	ZeroMemory(&depthStencilViewDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
	depthStencilViewDesc.Format = DXGI_FORMAT_D32_FLOAT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	hr = device->CreateDepthStencilView(texture_, &depthStencilViewDesc, &shadowStencilView);

	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
	ZeroMemory(&shaderResourceViewDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	shaderResourceViewDesc.Format = DXGI_FORMAT_R32_FLOAT;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;

	shaderResourceViewDesc.Texture2D.MipLevels = 1;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;

	hr = device->CreateShaderResourceView(texture_, &shaderResourceViewDesc, &shadowResourceView);

	D3D11_DEPTH_STENCIL_DESC depthstencildesc2;
	ZeroMemory(&depthstencildesc2, sizeof(D3D11_DEPTH_STENCIL_DESC));

	depthstencildesc2.DepthEnable = true;
	depthstencildesc2.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthstencildesc2.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

	hr = this->device->CreateDepthStencilState(&depthstencildesc2, this->shadowStencilState.GetAddressOf());

	D3D11_VIEWPORT viewport = {};
	viewport.Width = static_cast<float>(800);
	viewport.Height = static_cast<float>(800);
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.MinDepth = 0;
	viewport.MaxDepth = 1.0f;

	context->RSSetViewports(1, &viewport);

	for (auto component = gameComponents.begin(); component != gameComponents.end(); component++)
	{
		component->get()->Initialize();
		auto res = dynamic_cast<Player*>((*component).get());
		if (res != nullptr)
			cameraController = new CameraController(camera, this, static_cast<Player*>((*component).get()));
	}
}

void Game::StartGameLoop()
{
	PrevTime = std::chrono::steady_clock::now();

	while (wInput->ProcessMessages())
	{
		Update();
		GetInput();
		ShadowRender();
	}
}

void Game::AddGameComponent(std::shared_ptr<GameComponent> gc)
{
	gameComponents.push_back(std::move(gc));
}


#pragma region GameLoopPattern

void Game::GetInput()
{
	wInput->GetInput();
}

void Game::ShadowRender()
{
	context->ClearDepthStencilView(this->shadowStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	dirLight->GenerateViewMatrix();
	dirLight->GenerateOrthoFromFrustum(camera->GetViewMatrix(), camera->GetProjectionMatrix());

	renderTexture->SetRenderTarget(context.Get(), shadowStencilView.Get());

	renderTexture->m_renderTargetTexture = nullptr;


	DirectX::XMMATRIX world = DirectX::XMMatrixIdentity();
	worldViewData.worldViewProj = world * camera->GetViewMatrix() * camera->GetProjectionMatrix();


	for (auto component = gameComponents.begin(); component != gameComponents.end(); component++)
		component->get()->DepthPass();


	context->OMSetRenderTargets(1, rtv.GetAddressOf(), nullptr);
	context->OMSetRenderTargets(1, rtv.GetAddressOf(), this->depthStencilView.Get());

	float bgColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
	context->ClearRenderTargetView(rtv.Get(), bgColor);
	context->ClearDepthStencilView(this->depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);


	world = DirectX::XMMatrixIdentity();

	worldViewData.worldViewProj = world * camera->GetViewMatrix() * camera->GetProjectionMatrix();


	for (auto component = gameComponents.begin(); component != gameComponents.end(); component++)
		component->get()->MainPass();


	context->OMSetDepthStencilState(depthStencilState.Get(), 1);

	swapChain->Present(1, 0);
}


void Game::Render()
{
	float bgColor[] = {0.0f, 0.0f, 0.0f, 1.0f};
	context->ClearRenderTargetView(rtv.Get(), bgColor);

	context->ClearDepthStencilView(this->depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	DirectX::XMMATRIX world = DirectX::XMMatrixIdentity();
	worldViewData.worldViewProj = world * camera->GetViewMatrix() * camera->GetProjectionMatrix();


	for (auto component = gameComponents.begin(); component != gameComponents.end(); component++)
		component->get()->Render();


	swapChain->Present(1, 0);
}

void Game::Update()
{
	auto curTime = std::chrono::steady_clock::now();
	deltaTime = std::chrono::duration_cast<std::chrono::microseconds>(curTime - PrevTime).count() / 1000000.0f;
	PrevTime = curTime;

	totalTime += deltaTime;
	frameCount++;

	if (totalTime > 1.0f)
	{
		float fps = frameCount / totalTime;

		totalTime -= 1.0f;

		WCHAR text[256];
		swprintf_s(text, TEXT("FPS: %f"), fps);
		SetWindowText(GetWindow32HWND(), text);

		frameCount = 0;
	}

	for (auto component = gameComponents.begin(); component != gameComponents.end(); component++)
		component->get()->Update(deltaTime);

	cameraController->CameraMovement(deltaTime);
}

#pragma endregion
