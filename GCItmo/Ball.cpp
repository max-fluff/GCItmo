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

void Ball::Init()
{
	SetVelocity(Vector2(0.5f, 0.0f));
}

void Ball::OnCollision(Vector2 normal, Vector2 point, Physics2DObject* other)
{
	auto otherCasted = dynamic_cast<RectObject*>(other);
	if (!otherCasted)
		return;

	normal.Normalize();

	const auto reflectionVector = currentVelocity - 2.0f * currentVelocity.Dot(normal) * normal;

	SetVelocity(reflectionVector);
}

void Ball::Reset(bool throwRight)
{
	Pos position;
	position.x = 0;
	position.y = 0;
	SetPosition(position);

	SetVelocity(Vector2(0.5f * throwRight ? 1 : -1, 0.0f));
}
