#pragma once
#include "GeometricPrimitive.h"
#include "../Graphics/Shader.h"

#include <vector>

class Model3D;

class ModelShader:public Shader
{
public:
    ModelShader(std::shared_ptr<Game> _game,Model3D* _gc,LPCWSTR _filepath);
    
    void Initialize(LPCWSTR shaderPath) override;
    void Render() override;
    void GenerateSphere(float r,int segments,int slices);

    std::vector< DirectX::XMFLOAT4 > verts;
    std::vector< int > index;
    Model3D* gc;
    UINT strides[1] = { 48 }; 
    UINT offsets[1] = { 0 };
    Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerState;
    Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerDepthState;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> myTexture;
    
    LPCWSTR filepath;
    
};
