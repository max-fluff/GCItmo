#include "Player.h"

#include <iostream>

#include "Ball.h"

Player::Player(Game* game,
               Vertex* vertex,
               float width,
               float height,
               InputDevice* inputDevice,
               const unsigned char downKey,
               const unsigned char upKey,
               const float minPosition,
               const float maxPosition)
	: RectObject(game,
	             vertex,
	             width,
	             height), inputDevice(inputDevice),
	  downKey(downKey),
	  upKey(upKey),
	  minPosition(minPosition),
	  maxPosition(maxPosition)
{
}

void Player::Update(float deltaTime)
{
	const auto moveDirection = inputDevice->KeyIsPressed(downKey) ? -1 : 0 + inputDevice->KeyIsPressed(upKey) ? 1 : 0;
	auto currentPos = GetPositionY();
	currentPos += static_cast<float>(moveDirection) * SPEED * deltaTime;
	currentPos = currentPos < minPosition ? minPosition : currentPos;
	currentPos = currentPos > maxPosition ? maxPosition : currentPos;

	SetPositionY(currentPos);
}

void Player::OnCollision(Vector2 normal, Vector2 point, Physics2DObject* other)
{
	const auto ball = dynamic_cast<Ball*>(other);
	if (ball)
	{
		auto ballVelocity = ball->GetVelocity();
		//const auto y = ballVelocity.y - 0.1f;
		//ballVelocity.y = y;
		ball->SetVelocity(ballVelocity * 1.1f);
	}
}
