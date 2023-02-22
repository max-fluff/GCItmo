#include "Player.h"

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
