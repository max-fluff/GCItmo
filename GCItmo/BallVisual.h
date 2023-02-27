#pragma once
#include "CircleComponent.h"
#include "Ball.h"
class BallVisual :	public CircleComponent
{
private:
	Ball* ballToFollow;
public:
	BallVisual(Game* game, Vertex* vertex, float radius, int vertexAmount, float screenRatio, Ball* ballToFollow);
	void Update(float deltaTime) override;
};

