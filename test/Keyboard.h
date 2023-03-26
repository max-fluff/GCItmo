#pragma once
#include "InputEvents.h"
#include "Keys.h"
#include <unordered_set>

class Keyboard
{
public:
	Keyboard();
	void OnKeyDown(InputEvents::KeyboardEvent args);
	~Keyboard();
	void AddPressedKey(Keys key);
	void RemovePressedKey(Keys key);
	bool IsKeyDown(Keys key);

	
private:
	
	std::unordered_set<Keys>* keys;


};

