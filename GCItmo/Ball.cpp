#include "Ball.h"

Ball::Ball(RectObject* ball): ball(ball)
{
}

void Ball::Update(float deltaTime)
{
	if (ball == nullptr) return;
	auto pos = ball->GetPosition();
	pos.x += currentVelocity.x * deltaTime;
	pos.y += currentVelocity.y * deltaTime;
	ball->SetPosition(pos);
}

void Ball::OnCollision(Vector2 normal, Vector2 point, GameComponent* other)
{
	auto otherCasted = dynamic_cast<RectObject*>(other);
	if (otherCasted)
		return;

	normal.Normalize();
	const auto reflectionVector = currentVelocity - 2 * currentVelocity.Dot(normal) * normal;
	SetVelocity(reflectionVector);
}

void Ball::SetVelocity(const Vector2 velocity)
{
	currentVelocity = velocity;
}

void Ball::Init()
{
	SetVelocity(Vector2(0.1f, 0.1f));
}
