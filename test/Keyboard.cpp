#include "Keyboard.h"

Keyboard::Keyboard()
{
	keys = new std::unordered_set<Keys>();
}

void Keyboard::OnKeyDown(InputEvents::KeyboardEvent args)
{

}

Keyboard::~Keyboard()
{
	delete keys;
}

void Keyboard::AddPressedKey(Keys key)
{
	keys->insert(key);
}

void Keyboard::RemovePressedKey(Keys key)
{
	keys->erase(key);
}

bool Keyboard::IsKeyDown(Keys key)
{
	return keys->count(key);
}

