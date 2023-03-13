#include "Model3D.h"
#include "Player.h"
#include "../DXSDK/assimp/cimport.h"
#include "../DXSDK/assimp/postprocess.h"

Model3D::Model3D(Game* _game, std::string modelPath, LPCWSTR texturePath,
                 DirectX::SimpleMath::Vector3 pos, DirectX::SimpleMath::Vector3 scale)
{
	this->game = _game;
	this->texturePath = texturePath;
	this->modelPath = modelPath;
	this->pos = pos;
	this->scale = scale;
}

void Model3D::Init()
{
	UpdateWorldMatrix();
	std::string path = modelPath;

	constexpr auto structSize = sizeof(CB_VS_vertexshader);
	D3D11_BUFFER_DESC constantBufDesc = {};
	constantBufDesc.Usage = D3D11_USAGE_DYNAMIC;
	constantBufDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constantBufDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	constantBufDesc.MiscFlags = 0;

	constantBufDesc.ByteWidth = structSize + (16 - structSize) % 16;
	constantBufDesc.StructureByteStride = 0;

	game->device->CreateBuffer(&constantBufDesc, 0, &constantBuffer);

	shader = std::make_shared<ModelObject>(game, this, texturePath);
	shader->Init();
	Load(path);

	collider.Center = pos;
	collider.Extents.x = maxValues.x * scale.x;
	collider.Extents.y = maxValues.y * scale.y;
	collider.Extents.z = maxValues.z * scale.z;
}

void Model3D::SetTexture(ID3D11ShaderResourceView* texture)
{
	this->texture = texture;
}

bool Model3D::Load(std::string& filepath)
{
	Assimp::Importer importer;

	const aiScene* pScene = importer.ReadFile(filepath, aiProcess_Triangulate | aiProcess_ConvertToLeftHanded);

	if (pScene == nullptr)
		return false;

	this->ProcessNode(pScene->mRootNode, pScene);

	return true;
}

void Model3D::ProcessNode(const aiNode* node, const aiScene* scene)
{
	for (UINT i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(ProcessMesh(mesh));
	}

	for (UINT i = 0; i < node->mNumChildren; i++)
	{
		this->ProcessNode(node->mChildren[i], scene);
	}
}

Mesh* Model3D::ProcessMesh(aiMesh* mesh)
{
	std::vector<DirectX::XMFLOAT4> vertices;

	std::vector<int> indices;

	for (UINT i = 0; i < mesh->mNumVertices; i++)
	{
		DirectX::XMFLOAT4 vertex;
		DirectX::XMFLOAT4 color;
		vertex.x = mesh->mVertices[i].x;
		vertex.y = mesh->mVertices[i].y;
		vertex.z = mesh->mVertices[i].z;
		vertex.w = 1.0f;

		if (abs(vertex.x) > maxValues.x)
			maxValues.x = abs(vertex.x);
		if (abs(vertex.y) > maxValues.y)
			maxValues.y = abs(vertex.y);
		if (abs(vertex.z) > maxValues.z)
			maxValues.z = abs(vertex.z);

		if (mesh->mTextureCoords[0])
		{
			color.x = mesh->mTextureCoords[0][i].x;
			color.y = mesh->mTextureCoords[0][i].y;
		}

		vertices.push_back(vertex);
		vertices.push_back(DirectX::XMFLOAT4(color.x, color.y, 1.0f, 1));
	}

	for (UINT i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];

		for (UINT j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}

	return new Mesh(game, vertices, indices, shader.get());
}


void Model3D::UpdateWorldMatrix()
{
	worldMatrix = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z) * DirectX::XMMatrixTranslation(
		pos.x, pos.y, pos.z);
}

void Model3D::Draw()
{
	for (int i = 0; i < meshes.size(); i++)
	{
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		data.worldViewProj =
			worldMatrix *
			playerCoords *
			game->camera->GetViewMatrix() * game->camera->GetProjectionMatrix();

		game->context->Map(constantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		CopyMemory(mappedResource.pData, &data, sizeof(CB_VS_vertexshader));
		game->context->Unmap(constantBuffer, 0);
		game->context->VSSetConstantBuffers(0, 1, &constantBuffer);

		meshes[i]->Render();
	}
}

void Model3D::Update(float deltaSec)
{
}
