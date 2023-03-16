#pragma once
#include "DXSDK/DirectXHelpers.h"
#include "DXSDK/SimpleMath.h"

class DirectionalLight
{
public:
    DirectX::SimpleMath::Vector4 ambient;
    DirectX::SimpleMath::Vector4 light;
    DirectX::SimpleMath::Vector4 specular;
    DirectX::SimpleMath::Vector4 direction;
};
