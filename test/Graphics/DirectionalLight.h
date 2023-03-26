#pragma once
#include "DirectXHelpers.h"
#include "SimpleMath.h"


#include <vector>
using namespace DirectX::SimpleMath;

class DirectionalLight
{
public:

    void GenerateOrthoMatrix(float, float, float);
    void GenerateViewMatrix();
    void GenerateOrthoFromFrustum(const Matrix& view, const Matrix proj);
    std::vector<Vector4> GetFrustumCorners (const Matrix& view,const Matrix proj);
    std::vector<Matrix> GenerateOrthosFromFrustum(const Matrix& view, const Matrix proj,float far);


    std::vector<Matrix> viewMatrices;
    std::vector<Matrix> orthoMatrices;
    std::vector<Vector4> distances;
    
    DirectX::SimpleMath::Vector4 ambient;
    DirectX::SimpleMath::Vector4 light;
    DirectX::SimpleMath::Vector4 specular;
    DirectX::SimpleMath::Vector4 direction;

    DirectX::SimpleMath::Vector4 pos;
    DirectX::SimpleMath::Matrix viewMatrix;
    DirectX::SimpleMath::Matrix orthoMatrix;
    
};
