#pragma once

class GameComponent
{
public:
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
