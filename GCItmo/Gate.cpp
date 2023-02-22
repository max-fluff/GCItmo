#include "Gate.h"

#include <iostream>

void Gate::OnCollision(Vector2 normal, Vector2 point, Physics2DObject* other)
{
	std::cout << other->name;
}
