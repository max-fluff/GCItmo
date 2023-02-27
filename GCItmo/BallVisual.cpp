#include "BallVisual.h"

BallVisual::BallVisual(Game* game, Vertex* vertex, float radius, int vertexAmount, float screenRatio, Ball* ballToFollow) : CircleComponent(game, vertex, radius, vertexAmount, screenRatio), ballToFollow(ballToFollow)
{

}

void BallVisual::Update(float deltaTime)
{
	SetPosition(ballToFollow->GetPosition());
}
