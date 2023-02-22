#pragma once
#include <windows.h>
#include <WinUser.h>

#include "Game.h"

class DisplayWin32
{
	friend class Game;
public:
	int initClientHeight;
	int initClientWidth;
	WNDCLASSEX wc{};
	HWND hWnd;

	DisplayWin32(const int height, const int width, const LPCWSTR appName, Game* gameInstance);

	void SetAppWindowText(const WCHAR* text) const;
};
