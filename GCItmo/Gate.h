#pragma once
#include "RectObject.h"

class Gate: public RectObject
{
public:
	Gate(Game* game, Vertex* vertex, float width, float height)
		: RectObject(game, vertex, width, height)
	{
	}

	void OnCollision(DirectX::SimpleMath::Vector2 normal, DirectX::SimpleMath::Vector2 point,
	                 Physics2DObject* other) override;
};

