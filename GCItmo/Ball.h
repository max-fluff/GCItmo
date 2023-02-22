#pragma once
#include "GameComponent.h"
#include "RectObject.h"
#include "SimpleMath.h"

class Ball : public GameComponent
{
private:
	Vector2 currentVelocity;
	RectObject* ball;
public:
	Ball(RectObject* ball);
	bool isKinematic = false;
	void Update(float deltaTime) override;

	void OnCollision(Vector2 normal, Vector2 point, GameComponent* other);
	void SetVelocity(Vector2 velocity);
	void Init() override;
};
