#pragma once
#include <windows.h>
#include <WinUser.h>
#include <wrl.h>
#include <iostream>

inline LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch (umessage)
	{
	case WM_KEYDOWN:
		{
			// If a key is pressed send it to the input object so it can record that state.
			std::cout << "Key: " << static_cast<unsigned int>(wparam) << std::endl;

			if (static_cast<unsigned int>(wparam) == 27) PostQuitMessage(0);
			return 0;
		}
	default:
		{
			return DefWindowProc(hwnd, umessage, wparam, lparam);
		}
	}
}

class DisplayWin32
{
public:
	int client_height;
	int client_width;
	WNDCLASSEX wc{};
	HWND hWnd;

	DisplayWin32(const int height, const int width, const LPCWSTR appName)
	{
		client_height = height;
		client_width = width;

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

		RECT windowRect = {0, 0, static_cast<LONG>(client_width), static_cast<LONG>(client_height)};
		AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);

		constexpr auto dwStyle = WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX | WS_THICKFRAME;

		const auto posX = (GetSystemMetrics(SM_CXSCREEN) - client_width) / 2;
		const auto posY = (GetSystemMetrics(SM_CYSCREEN) - client_height) / 2;

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

	void SetAppWindowText(const WCHAR* text) const
	{
		SetWindowTextW(hWnd, text);
	}
};
