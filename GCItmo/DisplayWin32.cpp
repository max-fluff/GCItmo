#include "DisplayWin32.h"

#include <iostream>

Game* game;
DWORD lastTimeSinceStart = GetTickCount();

LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch (umessage)
	{
	case WM_KEYDOWN:
		{

			if (static_cast<unsigned int>(wparam) == 27) PostQuitMessage(0);
			return 0;
		}
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
