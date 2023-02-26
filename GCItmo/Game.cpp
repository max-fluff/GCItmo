#include "Game.h"
#include <iostream>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <chrono>

#include "Ball.h"
#include "DisplayWin32.h"
#include "Player.h"
#include "RectObject.h"
#include "ScoreComponent.h"

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

	display = new DisplayWin32(winHeight, winWidth, L"My3D App", this);

	inputDevice = new InputDevice();

	SetSwapDesc();
	CreateDeviceAndSwapChain();

	auto topWall = new RectObject(this, new Vertex(0.0f, 1.03f, 128, 128, 128, 1), 2.0f, 0.1f);
	topWall->name = "tW";
	auto bottomWall = new RectObject(this, new Vertex(0.0f, -1.03f, 128, 128, 128, 1), 2.0f, 0.1f);
	bottomWall->name = "bW";
	components.push_back(topWall);
	components.push_back(bottomWall);

	const auto leftPlayerBarVertex = new Vertex{-1.0f + 0.05f, 0.0f, 255, 128, 96, 1};
	const auto rightPlayerBarVertex = new Vertex{1.0f - 0.05f, 0.0f, 128, 64, 16, 1};

	constexpr auto playerHeight = 0.5f;
	auto minRacketPos = playerHeight / 2.0f - 1.0f + 0.02f;
	auto maxRacketPos = 1.0f - playerHeight / 2.0f - 0.02f;

	auto player1 = new Player(this, leftPlayerBarVertex, 0.05f, playerHeight, inputDevice, 'S', 'W',
	                          minRacketPos,
	                          maxRacketPos);
	player1->name = "player1";

	auto player2 = new Player(this, rightPlayerBarVertex, 0.05f, playerHeight, inputDevice, VK_DOWN, VK_UP,
	                          minRacketPos,
	                          maxRacketPos);
	player2->name = "player2";

	components.push_back(player1);
	components.push_back(player2);

	const auto gate1Vertex = new Vertex{-1.03f, 0.0f, 128, 64, 16, 1};
	const auto gate2Vertex = new Vertex{1.03f, 0.0f, 128, 64, 16, 1};

	auto gate1 = new RectObject(this, gate1Vertex, 0.1f, 1.96f);
	gate1->generateCollisions = false;
	auto gate2 = new RectObject(this, gate2Vertex, 0.1f, 1.96f);
	gate2->generateCollisions = false;

	components.push_back(gate1);
	components.push_back(gate2);

	auto ball = new Ball(this, new Vertex{0.0f, 0.0f, 255, 255, 255, 1}, 30.0f / winWidth, 30.0f / winHeight);
	ball->name = "ball";

	components.push_back(ball);

	components.push_back(new ScoreComponent(ball));

	for (const auto component : components)
		component->Init();
}

void Game::Update(float deltaTime)
{
	totalTime += deltaTime;

	if (totalTime > 1.0f)
		totalTime = 0;

	ProcessCollisions();

	for (const auto component : components)
		component->Update(deltaTime);

	PreDraw();

	for (const auto component : components)
		component->Draw();

	PostDraw();
}

void Game::ProcessCollisions()
{
	for (const auto element : components)
	{
		auto elemCasted = dynamic_cast<RectObject*>(element);

		if (elemCasted)
		{
			for (const auto elementOther : components)
			{
				auto elementOtherCasted = dynamic_cast<RectObject*>(elementOther);
				if (elementOtherCasted && elementOtherCasted != elemCasted && elementOtherCasted->generateCollisions)
				{
					int indexIfCollided = -1;

					for (int i = 0; i < collidedPair.size(); i++)
					{
						auto pair = collidedPair[i];
						if (pair.first == elemCasted && pair.second == elementOtherCasted)
						{
							indexIfCollided = i;
							break;
						}
					}


					auto rect1 = *new struct Rectangle(
						(elemCasted->GetPositionX() - elemCasted->GetWidth() / 2) * 1000,
						(elemCasted->GetPositionY() - elemCasted->GetHeight() / 2) * 1000,
						elemCasted->GetWidth() * 1000,
						elemCasted->GetHeight() * 1000);
					auto rect2 = *new struct Rectangle(
						(elementOtherCasted->GetPositionX() - elementOtherCasted->GetWidth() / 2) * 1000,
						(elementOtherCasted->GetPositionY() - elementOtherCasted->GetHeight() / 2) * 1000,
						elementOtherCasted->GetWidth() * 1000,
						elementOtherCasted->GetHeight() * 1000);

					if (rect1.Intersects(rect2))
					{
						if (indexIfCollided >= 0) continue;

						const auto intersectRect = Rectangle::Intersect(rect1, rect2);
						const auto collisionPoint = Vector2((intersectRect.x + intersectRect.width / 2.0f) / 1000.f,
						                                    (intersectRect.y + intersectRect.height / 2.0f) / 1000.f);

						elemCasted->OnCollision(
							Vector2(collisionPoint.x - elemCasted->GetPositionX(),
							        collisionPoint.y - elemCasted->GetPositionY()),
							collisionPoint,
							elementOtherCasted);
						collidedPair.push_back(
							std::pair<Physics2DObject*, Physics2DObject*>(elemCasted, elementOtherCasted));
					}
					else
					{
						if (indexIfCollided >= 0)
							collidedPair.erase(std::next(collidedPair.begin(), indexIfCollided));
					}
				}
			}
		}
	}
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
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (msg.message == WM_QUIT)
		{
			isExitRequested = true;
		}
	}
}
