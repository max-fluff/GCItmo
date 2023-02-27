#pragma once
#include <d3dcompiler.h>
#include <d3d11.h>
#include <wrl/client.h>

#include "SimpleMath.h"
#include "Vertex.h"

#include "Physics2DObject.h"

using namespace Microsoft::WRL;
using namespace DirectX::SimpleMath;

class Game;

class CircleComponent : public Physics2DObject
{
protected:
	Game* game;
	ID3D11Buffer* pIndexBuffer;
	ID3D11Buffer* pVertexBuffer;
	ComPtr<ID3D11InputLayout> pInputLayout;
	ComPtr<ID3D11VertexShader> pVertexShader;
	ComPtr<ID3D11PixelShader> pPixelShader;
	ComPtr<ID3D11Buffer> pConstantBuffer;

	Vertex* startVertex;
	float localPosX = 0;
	float localPosY = 0;
	float radius;
	int vertexAmount;
public:
	CircleComponent(Game* game, Vertex* vertex, float radius, int vertexAmount, float screenRatio);
	void Draw() override;
	Pos GetPosition();
	float GetPositionX();
	float GetPositionY();
	void SetPosition(Pos pos);
	void SetPositionX(float posX);
	void SetPositionY(float posY);
	float GetRadius();
};