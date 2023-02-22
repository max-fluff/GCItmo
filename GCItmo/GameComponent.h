#pragma once
#include <string>

class GameComponent
{
public:
	std::string name = "";

	virtual ~GameComponent() = default;

	virtual void Init()
	{
	}

	virtual void Update(float deltaTime)
	{
	}

	virtual void Draw()
	{
	}
};