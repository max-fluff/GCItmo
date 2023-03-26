#pragma once
#include "GeometricPrimitive.h"
#include "../Graphics/Shader.h"


#include <vector>

#include "assimp/mesh.h"
#include "assimp/scene.h"

class Mesh;
class Player;

class SphereShader: public Shader
{
public:
    SphereShader(std::shared_ptr<Game> _game,Player* _gc,LPCWSTR _filepath);
    
    void Initialize(LPCWSTR shaderPath) override;
    void Render() override;
    void GenerateSphere(float r,int segments,int slices);

    Mesh* ProcessMesh(aiMesh* mesh, const aiScene* scene);
    void Load(std::string& filepath);
    void ProcessNode(aiNode* node, const aiScene* scene);

    std::vector< DirectX::XMFLOAT4 > verts;
    std::vector< int > index;
    std::vector<Mesh*> meshes;

    
    Player* gc;
    UINT strides[1] = {48 }; 
    UINT offsets[1] = { 0 };
    Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerState;
    Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerDepthState;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> myTexture;
    LPCWSTR filepath;
    
};
