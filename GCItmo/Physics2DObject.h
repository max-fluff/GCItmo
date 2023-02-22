#pragma once
#include "GameComponent.h"
#include "SimpleMath.h"

class Physics2DObject: public GameComponent
{
	DirectX::SimpleMath::Vector2 currentVelocity;
public:
	bool isKinematic = false;

	void OnCollision(DirectX::SimpleMath::Vector2 normal, DirectX::SimpleMath::Vector2 point, Physics2DObject* other);
	void SetVelocity(DirectX::SimpleMath::Vector2 velocity);
};

