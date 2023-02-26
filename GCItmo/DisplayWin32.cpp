#include "DisplayWin32.h"

#include <iostream>

Game* game;
DWORD lastTimeSinceStart = GetTickCount();

LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	if (game == nullptr) return 0;

	switch (umessage)
	{
	case WM_DESTROY:
	case WM_CLOSE:
		{
			PostQuitMessage(0);
			return 0;
		}

	case WM_SIZE:
		{
			std::cout << "Width " << LOWORD(lparam) << " Height " << HIWORD(lparam) << std::endl;

			return 0;
		}

	case WM_PAINT:
		{
			const DWORD dwTimeCur = GetTickCount();

			const auto deltaTime = (dwTimeCur - lastTimeSinceStart) / 1000.0f;

			lastTimeSinceStart = dwTimeCur;

			game->Update(deltaTime);
			return 0;
		}

	case WM_KILLFOCUS:
		game->inputDevice->ClearState();
		break;
	case WM_SYSKEYDOWN:
	case WM_KEYDOWN:
		if (!(lparam & 0x40000000) || game->inputDevice->AutorepeatIsEnabled())
		{
			game->inputDevice->OnKeyPressed(static_cast<unsigned char>(wparam));
		}
		game->inputDevice->OnKeyPressed(static_cast<unsigned char>(wparam));

	//	std::cout << "Key: " << (unsigned int)wparam << std::endl;

		if ((unsigned int)wparam == 27) PostQuitMessage(0);
		break;
	case WM_SYSKEYUP:
	case WM_KEYUP:
		game->inputDevice->OnKeyReleased(static_cast<unsigned char>(wparam));
		break;
	case WM_CHAR:
		game->inputDevice->OnChar(static_cast<unsigned char>(wparam));
		break;
	//End Input Device

	case WM_LBUTTONDOWN:
		{
			return 0;
		}

	// All other messages pass to the message handler in the system class.
	default:
		{
			return DefWindowProc(hwnd, umessage, wparam, lparam);
		}
	}
}

DisplayWin32::DisplayWin32(const int height, const int width, const LPCWSTR appName, Game* gameInstance)
{
	game = gameInstance;

	initClientHeight = height;
	initClientWidth = width;

	const LPCWSTR applicationName = appName;
	const HINSTANCE hInstance = GetModuleHandle(nullptr);

	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(nullptr, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hbrBackground = static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = applicationName;
	wc.cbSize = sizeof(WNDCLASSEX);

	RegisterClassEx(&wc);

	RECT windowRect = {0, 0, static_cast<LONG>(initClientWidth), static_cast<LONG>(initClientHeight)};
	AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);

	constexpr auto dwStyle = WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX | WS_THICKFRAME;

	const auto posX = (GetSystemMetrics(SM_CXSCREEN) - initClientWidth) / 2;
	const auto posY = (GetSystemMetrics(SM_CYSCREEN) - initClientHeight) / 2;

	hWnd = CreateWindowEx(WS_EX_APPWINDOW, applicationName, applicationName,
	                      dwStyle,
	                      posX, posY,
	                      windowRect.right - windowRect.left,
	                      windowRect.bottom - windowRect.top,
	                      nullptr, nullptr, hInstance, nullptr);

	ShowWindow(hWnd, SW_SHOW);
	SetForegroundWindow(hWnd);
	SetFocus(hWnd);

	ShowCursor(true);
}

void DisplayWin32::SetAppWindowText(const WCHAR* text) const
{
	SetWindowTextW(hWnd, text);
}
