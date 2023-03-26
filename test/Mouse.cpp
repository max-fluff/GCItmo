#include "Mouse.h"

Mouse::Mouse()
{
	/*if (raw_input_initialized == false)
	{
		RAWINPUTDEVICE rid;
		rid.usUsagePage = 0x01;
		rid.usUsage = 0x02;
		rid.dwFlags = 0;
		rid.hwndTarget = NULL;

		if (RegisterRawInputDevices(&rid, 1, sizeof(rid)) == FALSE)
		{
			std::cout << "Failed to register device" << std::endl;
			exit(-1);
		}
	}
	raw_input_initialized = true;*/
}




void Mouse::OnMouseMove(int x, int y)
{
	this->prevX = this->x;
	this->prevY = this->y;
	this->x = x;
	this->y = y;
	this->rawOffset.x = (this->x - this->prevX);
	this->rawOffset.y = (this->y - this->prevY);

}

void Mouse::OnMouseMoveRaw(int x, int y)
{

	
}

bool Mouse::IsLeftDown()
{
	return this->leftIsDown;
}

bool Mouse::IsRightDown()
{
	return this->rightIsDown;
}

bool Mouse::IsMiddleDown()
{
	return this->mbuttonDown;
}

int Mouse::GetPosX()
{
	return this->x;
}

int Mouse::GetPosY()
{
	return this->y;
}

MousePoint Mouse::GetPos()
{
	return { this->x,this->y };
}

