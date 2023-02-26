#pragma once
#include "Physics2DObject.h"
#include "RectObject.h"
#include "SimpleMath.h"

class Ball : public RectObject
{
public:
	Ball(Game* game,
	     Vertex* vertex,
	     float width,
	     float height);
	void Update(float deltaTime) override;
	void Init() override;
	void OnCollision(Vector2 normal, Vector2 point, Physics2DObject* other) override;
	void Reset(bool throwRight);
};
