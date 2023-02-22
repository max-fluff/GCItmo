#pragma once
#include "Physics2DObject.h"
#include "RectObject.h"
#include "SimpleMath.h"

class Ball : public RectObject
{
private:
	Vector2 currentVelocity;
public:
	Ball(Game* game,
	     Vertex* vertex,
	     float width,
	     float height);
	void Update(float deltaTime) override;
	void SetVelocity(Vector2 velocity) override;
	void Init() override;
	void OnCollision(Vector2 normal, Vector2 point, Physics2DObject* other) override;
};
