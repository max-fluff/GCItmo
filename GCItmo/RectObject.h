#pragma once
#include <d3d11.h>
#include "SimpleMath.h"
#include "Vertex.h"
#include <wrl/client.h>

#include "GameComponent.h"

using namespace Microsoft::WRL;
using namespace DirectX::SimpleMath;

class Game;

class RectObject: public GameComponent
{
public:
	RectObject(Game* game,
		Vertex* vertex,
		float width,
		float height);
	void Draw() override;
	Pos GetPosition();
	float GetPositionX();
	float GetPositionY();
	void SetPosition(Pos pos);
	void SetPositionX(float posX);
	void SetPositionY(float posY);
	float GetWidth();
	float GetHeight();
	Pos GetLeftTop();
	Pos GetRightTop();
	Pos GetLeftBottom();
	Pos GetRightBottom();
private:
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
	float width = 0;
	float height = 0;
};

