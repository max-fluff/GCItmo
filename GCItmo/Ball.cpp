#include "Ball.h"

Ball::Ball(Game* game,
           Vertex* vertex,
           float width,
           float height): RectObject(game, vertex, width, height)
{
}

void Ball::Update(float deltaTime)
{
	auto pos = GetPosition();
	pos.x += currentVelocity.x * deltaTime;
	pos.y += currentVelocity.y * deltaTime;
	SetPosition(pos);
}

void Ball::SetVelocity(const Vector2 velocity)
{
	currentVelocity = velocity;
}

void Ball::Init()
{
	SetVelocity(Vector2(0.1f, 0.1f));
}

void Ball::OnCollision(Vector2 normal, Vector2 point, Physics2DObject* other)
{
	auto otherCasted = dynamic_cast<RectObject*>(other);
	if (otherCasted)
		return;

	normal.Normalize();
	const auto reflectionVector = currentVelocity - 2 * currentVelocity.Dot(normal) * normal;
	SetVelocity(reflectionVector);
}
