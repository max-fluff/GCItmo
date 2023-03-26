#pragma once
#include "../../RenderComponents/GameComponent.h"
#include "DirectXCollision.h"
#include <GeometricPrimitive.h>

#include "SimpleMath.h"

class MyCube: public GameComponent
{
public:
    MyCube(std::shared_ptr<Game>,DirectX::SimpleMath::Vector3 startPos);
    void Initialize() override;
    void Render() override;
    void Update(float deltaSec) override;
    void MainPass() override{};
    void DepthPass() override{};

    DirectX::BoundingBox collider;
    DirectX::XMMATRIX worldMatrix;
    DirectX::XMMATRIX playerCoords=DirectX::XMMatrixIdentity();
    DirectX::SimpleMath::Vector3 pos;

    bool isSticked=false;
    
private:
    std::unique_ptr<DirectX::GeometricPrimitive> cube;
    
};
