#pragma once

#include <windows.h>
#include <WinUser.h>


class Game;

class DisplayWin32
{
public:
	DisplayWin32(Game* _game,LPCWSTR applicationName, HINSTANCE hInstance, int screenHeight = 800, int screenWidth = 800);

	~DisplayWin32();

	HWND GetHWND();

	int GetHeight();

	int GetWidth();



private:
	

	int clientHeight;
	int clientWidth;
	HINSTANCE hInstance;
	HWND hWnd;
	WNDCLASSEX wc;

	Game* game;

	

};

