#pragma once
#include "../GameComponent.h"
#include "../DXSDK/GeometricPrimitive.h"
#include <memory>


class Game;

class DebugPlane:public GameComponent
{
public:

    DebugPlane(Game* game);

    void Init() override;
    void Draw() override;

private:
    std::unique_ptr<DirectX::GeometricPrimitive> plane;
    Game* game;
};
