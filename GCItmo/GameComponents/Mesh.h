#pragma once
#include <DirectXMath.h>
#include <vector>

#include "ModelObject.h"

class Game;

class Mesh
{
public:
	Mesh(Game* _game, std::vector<DirectX::XMFLOAT4>& vertices, std::vector<int>& indices, ModelObject* sh);
	void Render();
	void Initialize();
	DirectX::XMMATRIX transformMatrix;
private:
	ID3D11Buffer* pIndexBuffer;
	ID3D11Buffer* pVertexBuffer;
	int indicesSize;

	Game* game;
	ModelObject* shader;
};
