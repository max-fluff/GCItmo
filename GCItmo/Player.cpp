#include "Player.h"

#include <iostream>

Player::Player(InputDevice* inputDevice,
               RectObject* controllableObject,
               const unsigned char downKey,
               const unsigned char upKey,
               const float minPosition,
               const float maxPosition)
	: inputDevice(inputDevice),
	  downKey(downKey),
	  upKey(upKey),
	  controllableObject(controllableObject),
	  minPosition(minPosition),
	  maxPosition(maxPosition)
{
}

void Player::Update(float deltaTime)
{
	const auto moveDirection = inputDevice->KeyIsPressed(downKey) ? -1 : 0 + inputDevice->KeyIsPressed(upKey) ? 1 : 0;
	auto currentPos = controllableObject->GetPositionY();
	currentPos += static_cast<float>(moveDirection) * SPEED * deltaTime;
	currentPos = currentPos < minPosition ? minPosition : currentPos;
	currentPos = currentPos > maxPosition ? maxPosition : currentPos;

	controllableObject->SetPositionY(currentPos);
}
