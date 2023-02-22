#include "Physics2DObject.h"

void Physics2DObject::OnCollision(DirectX::SimpleMath::Vector2 normal, DirectX::SimpleMath::Vector2 point,
	Physics2DObject* other)
{
}

void Physics2DObject::SetVelocity(DirectX::SimpleMath::Vector2 velocity)
{
	currentVelocity = velocity;
}

DirectX::SimpleMath::Vector2 Physics2DObject::GetVelocity()
{
	return currentVelocity;
}
