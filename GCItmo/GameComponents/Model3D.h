#pragma once

#include <d3d.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include "DirectXCollision.h"
#include "../GameComponent.h"
#include "../Game.h"
#include "../DXSDK/assimp/scene.h"
#include "../DXSDK/assimp/Importer.hpp"

#include <vector>

#include "Mesh.h"

namespace DirectX
{
	namespace SimpleMath
	{
		struct Vector3;
	}
}

class Model3D : public GameComponent
{
public:
	Model3D(Game* _game, std::string modelPath, LPCWSTR texturePath, DirectX::SimpleMath::Vector3 pos,
	        DirectX::SimpleMath::Vector3 scale = DirectX::SimpleMath::Vector3(1.0f, 1.0f, 1.0f));
	void SetTexture(ID3D11ShaderResourceView* texture);
	bool Load(std::string& filepath);
	void ProcessNode(const aiNode* node, const aiScene* scene);
	Mesh* ProcessMesh(aiMesh* mesh);

	void Init() override;
	void Draw() override;
	void Update(float deltaSec) override;


	DirectX::SimpleMath::Vector3 pos;
	DirectX::SimpleMath::Vector3 rot;
	UINT* strides;
	UINT* offsets;
	DirectX::BoundingBox collider;
	DirectX::XMMATRIX worldMatrix = DirectX::XMMatrixIdentity();
	DirectX::XMMATRIX playerCoords = DirectX::XMMatrixIdentity();
	DirectX::SimpleMath::Vector3 scale;
	bool isSticked = false;
private:
	void UpdateWorldMatrix();

	ID3D11Buffer* constantBuffer;
	ID3D11ShaderResourceView* texture = nullptr;

	CB_VS_vertexshader data;

	Game* game;

	std::vector<Mesh*> meshes;
	std::shared_ptr<ModelObject> shader;

	LPCWSTR texturePath;
	std::string modelPath;

	DirectX::SimpleMath::Vector3 maxValues = DirectX::SimpleMath::Vector3(0, 0, 0);
	DirectX::SimpleMath::Vector3 minValues = DirectX::SimpleMath::Vector3(100000.0f, 100000.0f, 100000.0f);
};
