#include "Game.h"
#include <iostream>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <chrono>

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
	swapDesc.BufferDesc.Width = display->client_width;
	swapDesc.BufferDesc.Height = display->client_height;
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

	auto res = D3D11CreateDeviceAndSwapChain(
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


	res = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backTex);
	res = device->CreateRenderTargetView(backTex, nullptr, &renderTargetView);
}

void Game::CompileFromFile()
{
	vertexBC = nullptr;
	errorVertexCode = nullptr;
	const auto res = D3DCompileFromFile(fileName,
	                                    nullptr /*macros*/,
	                                    nullptr /*include*/,
	                                    "VSMain",
	                                    "vs_5_0",
	                                    D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
	                                    0,
	                                    &vertexBC,
	                                    &errorVertexCode);

	if (FAILED(res))
	{
		// If the shader failed to compile it should have written something to the error message.
		if (errorVertexCode)
		{
			char* compileErrors = (char*)(errorVertexCode->GetBufferPointer());

			std::cout << compileErrors << std::endl;
		}
		// If there was  nothing in the error message then it simply could not find the shader file itself.
		else
		{
			MessageBox(display->hWnd, fileName, L"Missing Shader File", MB_OK);
		}
	}
}

void Game::Render()
{
	D3D_SHADER_MACRO Shader_Macros[] = {"TEST", "1", "TCOLOR", "float4(0.0f, 1.0f, 0.0f, 1.0f)", nullptr, nullptr};

	ID3DBlob* pixelBC;
	ID3DBlob* errorPixelCode;
	auto res = D3DCompileFromFile(fileName, Shader_Macros /*macros*/, nullptr /*include*/,
	                              "PSMain", "ps_5_0", D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, &pixelBC,
	                              &errorPixelCode);

	device->CreateVertexShader(
		vertexBC->GetBufferPointer(),
		vertexBC->GetBufferSize(),
		nullptr, &vertexShader);

	device->CreatePixelShader(
		pixelBC->GetBufferPointer(),
		pixelBC->GetBufferSize(),
		nullptr, &pixelShader);

	D3D11_INPUT_ELEMENT_DESC inputElements[] = {
		D3D11_INPUT_ELEMENT_DESC{
			"POSITION",
			0,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			0,
			0,
			D3D11_INPUT_PER_VERTEX_DATA,
			0
		},
		D3D11_INPUT_ELEMENT_DESC{
			"COLOR",
			0,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			0,
			D3D11_APPEND_ALIGNED_ELEMENT,
			D3D11_INPUT_PER_VERTEX_DATA,
			0
		}
	};

	device->CreateInputLayout(
		inputElements,
		2,
		vertexBC->GetBufferPointer(),
		vertexBC->GetBufferSize(),
		&layout);

	DirectX::XMFLOAT4 points[8] = {
		DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f), DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f),
		DirectX::XMFLOAT4(-0.5f, -0.5f, 0.5f, 1.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f),
		DirectX::XMFLOAT4(0.5f, -0.5f, 0.5f, 1.0f), DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f),
		DirectX::XMFLOAT4(-0.5f, 0.5f, 0.5f, 1.0f), DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
	};


	D3D11_BUFFER_DESC vertexBufDesc = {};
	vertexBufDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufDesc.CPUAccessFlags = 0;
	vertexBufDesc.MiscFlags = 0;
	vertexBufDesc.StructureByteStride = 0;
	vertexBufDesc.ByteWidth = sizeof(DirectX::XMFLOAT4) * std::size(points);

	D3D11_SUBRESOURCE_DATA vertexData = {};
	vertexData.pSysMem = points;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	device->CreateBuffer(&vertexBufDesc, &vertexData, &vertexBuffer);

	int indexes[] = {0, 1, 2, 1, 0, 3};
	D3D11_BUFFER_DESC indexBufDesc = {};
	indexBufDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufDesc.CPUAccessFlags = 0;
	indexBufDesc.MiscFlags = 0;
	indexBufDesc.StructureByteStride = 0;
	indexBufDesc.ByteWidth = sizeof(int) * std::size(indexes);

	D3D11_SUBRESOURCE_DATA indexData = {};
	indexData.pSysMem = indexes;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	device->CreateBuffer(&indexBufDesc, &indexData, &indexBuffer);

	rastDesc.CullMode = D3D11_CULL_NONE;
	rastDesc.FillMode = D3D11_FILL_SOLID;

	res = device->CreateRasterizerState(&rastDesc, &rastState);

	context->RSSetState(rastState);
}

void Game::Initialize()
{
	constexpr auto winHeight = 800;
	constexpr auto winWidth = 800;

	display = new DisplayWin32(winHeight, winWidth, L"My3D App");

	SetSwapDesc();
	CreateDeviceAndSwapChain();
	CompileFromFile();
	Render();
}

void Game::Update(float deltaTime)
{
	context->ClearState();

	context->RSSetState(rastState);

	D3D11_VIEWPORT viewport = {};
	viewport.Width = static_cast<float>(display->client_width);
	viewport.Height = static_cast<float>(display->client_height);
	viewport.TopLeftX = 0;
	viewport.TopLeftY = totalTime * 10;
	viewport.MinDepth = 0;
	viewport.MaxDepth = 1.0f;

	context->RSSetViewports(1, &viewport);

	context->IASetInputLayout(layout);
	context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	context->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	context->IASetVertexBuffers(0, 1, &vertexBuffer, strides, offsets);
	context->VSSetShader(vertexShader, nullptr, 0);
	context->PSSetShader(pixelShader, nullptr, 0);

	totalTime += deltaTime;

	if (totalTime > 1.0f)
		totalTime = 0;

	context->OMSetRenderTargets(1, &renderTargetView, nullptr);

	const float color[] = {totalTime, 0.1f, 0.1f, 1.0f};
	context->ClearRenderTargetView(renderTargetView, color);

	context->DrawIndexed(6, 0, 0);

	context->OMSetRenderTargets(0, nullptr, nullptr);

	swapChain->Present(1, /*DXGI_PRESENT_DO_NOT_WAIT*/ 0);
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
		// Handle the windows messages.
		while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// If windows signals to end the application then exit out.
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
