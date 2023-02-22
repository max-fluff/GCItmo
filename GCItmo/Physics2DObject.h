#pragma once
#include "GameComponent.h"
#include <d3d11.h>
#include "SimpleMath.h"

class Physics2DObject : public GameComponent
{
protected:
	DirectX::SimpleMath::Vector2 currentVelocity;
public:
	bool isKinematic = false;

	virtual void OnCollision(DirectX::SimpleMath::Vector2 normal, DirectX::SimpleMath::Vector2 point,	                         Physics2DObject* other);
	virtual void SetVelocity(DirectX::SimpleMath::Vector2 velocity);
	virtual DirectX::SimpleMath::Vector2 GetVelocity();
};
