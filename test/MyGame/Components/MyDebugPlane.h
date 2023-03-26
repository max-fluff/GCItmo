#pragma once
#include "../../RenderComponents/GameComponent.h"
#include <GeometricPrimitive.h>


class MyDebugPlane:public GameComponent
{
public:

    MyDebugPlane(std::shared_ptr<Game> g);
    

    void Initialize() override;
    void Render() override;
    void Update(float deltaSec) override;
    void MainPass() override{};
    void DepthPass() override{};

private:
    std::unique_ptr<DirectX::GeometricPrimitive> plane;
};
