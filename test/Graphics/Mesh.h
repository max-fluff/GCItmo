#pragma once
#include "../Graphics/Buffer.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include "../MyGame/Shaders/DepthShader.h"
#include "../MyGame/Shaders/ModelShader.h"


class Game;

class Mesh
{
public:
    Mesh(std::shared_ptr<Game> _game,std::vector<DirectX::XMFLOAT4>& vertices,
        std::vector<int>& indices,ModelShader* sh,DepthShader* ds);
    Mesh(const Mesh & mesh);
    void Render();
    void RenderToTexture();
    void Initialize();
    void DepthPass();
    DirectX::XMMATRIX transformMatrix;
private:
    std::shared_ptr<Buffer> vertexBuffer;
    std::shared_ptr<Buffer> indexBuffer;
    std::shared_ptr<Game> game;
    ModelShader* shader;
    DepthShader* depthShader;
    
};
