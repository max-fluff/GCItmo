#pragma once
#include "GameComponent.h"
#include "InputDevice.h"
#include "RectObject.h"

class Player : public GameComponent
{
private:
	const float SPEED = 1; //percent*100 per second
	InputDevice* inputDevice;
	unsigned char downKey;
	unsigned char upKey;
	RectObject* controllableObject;
	float minPosition;
	float maxPosition;

public:
	Player(
		InputDevice* inputDevice,
		RectObject* controllableObject,
		unsigned char downKey,
		unsigned char upKey,
		float minPosition,
		float maxPosition);
	void Update(float deltaTime) override;
};
