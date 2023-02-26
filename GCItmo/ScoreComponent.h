#pragma once
#include "Ball.h"
#include "GameComponent.h"

class ScoreComponent : public GameComponent
{
private:
	int scorePlayer1 = 0;
	int scorePlayer2 = 0;
	Ball* ball;
public:
	explicit ScoreComponent(Ball* ball)
		: ball(ball)
	{
	}

	void Update(float deltaTime) override;
};
