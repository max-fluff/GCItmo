#pragma once
#include "../RenderComponents/GameComponent.h"

class Game;

class ShadowCaster: public GameComponent
{
public:
    
    void Initialize() override;
    void Update(float deltaSec) override;
    void Render() override;

private:
    std::shared_ptr<Game> game;
    
};
