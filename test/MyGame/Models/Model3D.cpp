#include "Model3D.h"
#include "../Components/Player.h"


Model3D::Model3D(std::shared_ptr<Game> _game, std::string modelPath, LPCWSTR texturePath,
                 Vector3 pos, Vector3 scale, Vector4 _mat)
{
	this->game = _game;
	this->texturePath = texturePath;
	this->modelPath = modelPath;
	this->pos = pos;
	this->scale = scale;
	this->material = std::make_shared<Material>(_mat);
}

void Model3D::Initialize()
{
	UpdateWorldMatrix();
	Assimp::Importer Importer;
	std::string path = modelPath;

	constantBuffer = std::make_shared<Buffer>(game);
	int k = sizeof(DirectX::XMMATRIX);
	constantBuffer->Initialize(sizeof(CB_ModelLightning), D3D11_USAGE_DYNAMIC);


	shader = std::make_shared<ModelShader>(game, this, texturePath);
	shader->Initialize(L"./Shaders/ModelShader.hlsl");
	depthShader = std::make_shared<DepthShader>(game);
	depthShader->Initialize(L"./Shaders/DepthShader.hlsl");
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

void Model3D::Draw(const DirectX::XMMATRIX& viewProjectionMatrix)
{
}


bool Model3D::Load(std::string& filepath)
{
	Assimp::Importer importer;

	const aiScene* pScene = importer.ReadFile(
		filepath, aiProcess_Triangulate | aiProcess_SortByPType | aiProcess_ConvertToLeftHanded);

	if (pScene == nullptr)
		return false;

	this->ProcessNode(pScene->mRootNode, pScene);


	return true;
}

void Model3D::ProcessNode(aiNode* node, const aiScene* scene)
{
	for (UINT i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(this->ProcessMesh(mesh, scene));
	}

	for (UINT i = 0; i < node->mNumChildren; i++)
	{
		this->ProcessNode(node->mChildren[i], scene);
	}
}

Mesh* Model3D::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<DirectX::XMFLOAT4> vertices;

	std::vector<int> indices;

	for (UINT i = 0; i < mesh->mNumVertices; i++)
	{
		DirectX::XMFLOAT4 vertex;
		DirectX::XMFLOAT4 color;
		DirectX::XMFLOAT4 normals = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
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

		normals.x = mesh->mNormals[i].x;
		normals.y = mesh->mNormals[i].y;
		normals.z = mesh->mNormals[i].z;


		vertices.push_back(vertex);
		vertices.push_back(DirectX::XMFLOAT4(color.x, color.y, 1.0f, 1));
		vertices.push_back(normals);
	}

	for (UINT i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];

		for (UINT j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}

	return new Mesh(game, vertices, indices, shader.get(), depthShader.get());
}


void Model3D::UpdateWorldMatrix()
{
	worldMatrix = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z) * DirectX::XMMatrixTranslation(
		pos.x, pos.y, pos.z);
}

void Model3D::Render()
{
	for (int i = 0; i < meshes.size(); i++)
	{
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		data.worldViewProj =
			worldMatrix *
			playerCoords *
			game->camera->GetViewMatrix() * game->camera->GetProjectionMatrix();

		data.worldInverseT =
			XMMatrixTranspose(
				XMMatrixInverse(nullptr,
				                worldMatrix * playerCoords));

		data.world = worldMatrix * playerCoords;

		data.ambient = game->dirLight.get()->ambient;
		data.light = game->dirLight.get()->light;
		data.specular = material->specularCoef;
		data.direction = game->dirLight.get()->direction;

		data.eyePos = Vector4(game->camera->pos.x, game->camera->pos.y, game->camera->pos.z, 1.0f);

		HRESULT res = game->context->Map(constantBuffer->buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		CopyMemory(mappedResource.pData, &data, sizeof(CB_ModelLightning));
		game->context->Unmap(constantBuffer->buffer.Get(), 0);
		game->context->VSSetConstantBuffers(0, 1, constantBuffer->buffer.GetAddressOf());
		game->context->PSSetConstantBuffers(0, 1, constantBuffer->buffer.GetAddressOf());
		meshes[i]->Render();
	}
}


void Model3D::MainPass()
{
	for (int i = 0; i < meshes.size(); i++)
	{
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		data.worldViewProj =
			worldMatrix *
			playerCoords *
			game->camera->GetViewMatrix() * game->camera->GetProjectionMatrix();

		data.lightViewProj = Matrix(worldMatrix) * Matrix(playerCoords) *
			game->dirLight->viewMatrix *
			game->dirLight->orthoMatrix;

		data.worldInverseT =
			XMMatrixTranspose(
				XMMatrixInverse(nullptr,
				                worldMatrix * playerCoords));

		data.world = worldMatrix * playerCoords;

		data.worldView = worldMatrix * playerCoords * game->camera->GetViewMatrix();

		data.ambient = game->dirLight.get()->ambient;
		data.light = game->dirLight.get()->light;
		data.specular = material->specularCoef;
		data.direction = game->dirLight.get()->direction;

		data.eyePos = Vector4(game->camera->pos.x, game->camera->pos.y, game->camera->pos.z, 1.0f);

		data.viewProj = game->dirLight->viewMatrix * game->dirLight->orthoMatrix;


		HRESULT res = game->context->Map(constantBuffer->buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		CopyMemory(mappedResource.pData, &data, sizeof(CB_ModelLightning));
		game->context->Unmap(constantBuffer->buffer.Get(), 0);
		game->context->VSSetConstantBuffers(0, 1, constantBuffer->buffer.GetAddressOf());
		game->context->PSSetConstantBuffers(0, 1, constantBuffer->buffer.GetAddressOf());


		meshes[i]->RenderToTexture();

		int qq = 0;
	}
}

void Model3D::DepthPass()
{
	for (int i = 0; i < meshes.size(); i++)
	{
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		data.worldViewProj =
			Matrix(worldMatrix) * Matrix(playerCoords) *
			game->dirLight->viewMatrix *
			game->dirLight->orthoMatrix;

		data.lightViewProj = Matrix(worldMatrix) * Matrix(playerCoords) *
			game->dirLight->viewMatrix *
			game->dirLight->orthoMatrix;

		data.worldInverseT =
			XMMatrixTranspose(
				XMMatrixInverse(nullptr,
				                worldMatrix * playerCoords));

		data.world = worldMatrix * playerCoords;

		data.worldView = worldMatrix * playerCoords * game->camera->GetViewMatrix();

		data.ambient = game->dirLight.get()->ambient;
		data.light = game->dirLight.get()->light;
		data.specular = material->specularCoef;
		data.direction = game->dirLight.get()->direction;

		data.eyePos = Vector4(game->camera->pos.x, game->camera->pos.y, game->camera->pos.z, 1.0f);
		data.viewProj = game->dirLight->viewMatrix * game->dirLight->orthoMatrix;


		HRESULT res = game->context->Map(constantBuffer->buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		CopyMemory(mappedResource.pData, &data, sizeof(CB_ModelLightning));
		game->context->Unmap(constantBuffer->buffer.Get(), 0);
		game->context->VSSetConstantBuffers(0, 1, constantBuffer->buffer.GetAddressOf());
		game->context->PSSetConstantBuffers(0, 1, constantBuffer->buffer.GetAddressOf());
		game->context->GSSetConstantBuffers(0, 1, constantBuffer->buffer.GetAddressOf());


		meshes[i]->DepthPass();

		int qq = 0;
	}
}


void Model3D::Update(float deltaSec)
{
}
