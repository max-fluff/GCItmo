#pragma once
#include <Windows.h>
#include "Keys.h"

class InputEvents
{
public:
	enum KeyState
	{
		Pressed,
		Released
	};

	struct WheelEvent
	{
		Keys key;

	};

	struct KeyboardEvent
	{
		KeyState state;
		Keys key;

		KeyboardEvent(KeyState ks, Keys key)
		{
			state = ks;
			this->key = key;
		}
	};
		
	

};

