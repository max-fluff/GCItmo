#pragma once
#include <d3dcommon.h>
#include <d3d11.h>
#include <directxmath.h>

#include "../GameComponent.h"

#include <vector>
#include <wrl/client.h>

class Game;
class Model3D;

class ModelObject: public GameComponent
{
    friend class Mesh;
public:
    ModelObject(Game* _game,Model3D* _gc,LPCWSTR _filepath);
    
    void Init() override;

    std::vector< DirectX::XMFLOAT4 > verts;
    std::vector< int > index;
    Model3D* gc;
    UINT strides[1] = { 32 }; 
    UINT offsets[1] = { 0 };
    Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerState;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> myTexture;
    
    LPCWSTR filepath;
private:
    ID3D11Buffer* constantBuffer;
	ID3D11Buffer* pIndexBuffer;
	ID3D11Buffer* pVertexBuffer;
    Game* game;

    LPCWSTR shaderPath = L"./Shaders/SphereShader.hlsl";

    Microsoft::WRL::ComPtr<ID3D11InputLayout> layout;
    Microsoft::WRL::ComPtr<ID3DBlob> vertexBC;
    Microsoft::WRL::ComPtr<ID3DBlob> pixelBC;

    Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader;
    Microsoft::WRL::ComPtr<ID3D11PixelShader> pixelShader;
    Microsoft::WRL::ComPtr<ID3D11RasterizerState> rastState;
};
