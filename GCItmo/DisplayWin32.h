#pragma once
#include <windows.h>
#include <WinUser.h>

class Game;

class DisplayWin32
{
public:
	int initClientHeight;
	int initClientWidth;
	WNDCLASSEX wc{};
	HWND hWnd;

	DisplayWin32(int height, const int width, const LPCWSTR appName, Game* gameInstance);

	void SetAppWindowText(const WCHAR* text) const;
};
