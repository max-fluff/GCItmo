#include "ScoreComponent.h"

#include <iostream>

void ScoreComponent::Update(float deltaTime)
{
	bool changes = false;

	if (ball->GetPositionX() > 1)
	{
		scorePlayer1++;
		changes = true;
		ball->Reset(false);
	}
	if (ball->GetPositionX() < -1)
	{
		scorePlayer2++;
		changes = true;
		ball->Reset(true);
	}
	if (changes)
	{
		std::cout << "GOAL " << "\n" << "Current Score: " << scorePlayer1 << "--" << scorePlayer2 <<
			"\n----------------------\n";
	}
}
