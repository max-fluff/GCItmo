#pragma once

#include "InputDevice.h"
#include "RectObject.h"

class Player : public RectObject
{
private:
	const float SPEED = 3; //percent*100 per second
	InputDevice* inputDevice;
	unsigned char downKey;
	unsigned char upKey;
	float minPosition;
	float maxPosition;

public:
	bool isKinematic = true;

	Player(Game* game,
	       Vertex* vertex,
	       float width,
	       float height,
	       InputDevice* inputDevice,
	       unsigned char downKey,
	       unsigned char upKey,
	       float minPosition,
	       float maxPosition);
	void Update(float deltaTime) override;
};
