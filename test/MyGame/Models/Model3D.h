#pragma once

#include <d3d.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include "DirectXCollision.h"
#include "../RenderComponents/GameComponent.h"
#include "../Game.h"
#include "assimp/scene.h"
#include "assimp/Importer.hpp"
#include "postprocess.h"
#include <d3d11.h>
#include <vector>
#include "../Shaders/ModelShader.h"
#include "../Graphics/Mesh.h"
#include "../Graphics/Material.h"
#include "../Shaders/DepthShader.h"


class Model3D: public GameComponent
{
public:
    Model3D(std::shared_ptr<Game> _game,std::string modelPath,LPCWSTR texturePath, Vector3 pos,
            Vector3 scale= Vector3(1.0f,1.0f,1.0f),
            Vector4 _spec= Vector4(1.0f,1.0f,1.0f,1.0f));
    void SetTexture(ID3D11ShaderResourceView* texture);
    void Draw(const DirectX::XMMATRIX& viewProjectionMatrix);
    bool Load(std::string& filepath);
    void ProcessNode(aiNode* node,const aiScene * scene);
    Mesh* ProcessMesh(aiMesh * mesh,const aiScene * scene);

    void Initialize() override;
    void Render() override;
    void MainPass() override;
    void DepthPass() override;
    void Update(float deltaSec) override;


    Vector3 pos;
    Vector3 rot;
    UINT* strides;
    UINT* offsets;
    DirectX::BoundingBox collider;
    DirectX::XMMATRIX worldMatrix = DirectX::XMMatrixIdentity();
    DirectX::XMMATRIX playerCoords=DirectX::XMMatrixIdentity();
    Vector3 scale;
    bool isSticked=false;
     std::shared_ptr<Material> material;
private:
    
    void UpdateWorldMatrix();

    std::shared_ptr<Buffer> constantBuffer;
    std::shared_ptr<Buffer> vertexBuffer;
    std::shared_ptr<Buffer> indexBuffer;
    ID3D11ShaderResourceView * texture = nullptr;

    CB_ModelLightning data;
    

    std::vector<Mesh*> meshes;
    std::shared_ptr<ModelShader> shader;
    std::shared_ptr<DepthShader> depthShader;

   

    LPCWSTR texturePath;
    std::string modelPath;

    Vector3 maxValues= Vector3(0,0,0);
    
};
