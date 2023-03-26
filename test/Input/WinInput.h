#pragma once
#include <windows.h>
#include <WinUser.h>
#include <wrl.h>
#include <iostream>
#include <d3d.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <chrono>
#include <unordered_set>
#include "Keys.h"
//#include "InputEvents.h"
#include "../DirectXSDK/Delegates.h"

class Game;

class WinInput
{

public:
	WinInput(Game* _game);
	~WinInput();


	void AddPressedKey(Keys key);
	void RemovePressedKey(Keys key);
	bool IsKeyDown(Keys key);

	void OnMouseMove(int x, int y);

	//Доделать 

	bool ProcessMessages();

	void GetInput();


	POINT GetMousePos();
	POINT GetRawOffset();

	MulticastDelegate<const Keys> WheelMove;
	MulticastDelegate<POINT> RawOffsetEvent;
	
private:
	
	Game* game;
	HWND windowHWND;
	std::unordered_set<Keys>* keys;
	POINT mousePos;
	POINT prevPos;
	POINT rawOffset;
	bool rawInputInitialized=false;
};

