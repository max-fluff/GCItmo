#pragma once
#include "MouseEvent.h"
#include <Windows.h>
#include <iostream>
#include <queue>

class Mouse
{

public:
	Mouse();

	void OnMouseMove(int x, int y);
	void OnMouseMoveRaw(int x, int y);

	bool IsLeftDown();
	bool IsMiddleDown();
	bool IsRightDown();

	int GetPosX();
	int GetPosY();
	MousePoint GetPos();



private:
	bool leftIsDown = false;
	bool rightIsDown = false;
	bool mbuttonDown = false;
	bool raw_input_initialized = false;
	int x = 0;
	int y = 0;
	int prevX = 0;
	int prevY = 0;
	POINT rawOffset;
};

