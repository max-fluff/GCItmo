#pragma once

#include "../GameComponent.h"
#include "../DXSDK/ConstantBufferTypes.h"
#include "../GameComponents/SphereObject.h"

#include <DirectXMath.h>

class Game;

class Planet : public GameComponent
{
public:
	Planet(Game* game, float radius, float axisAngle, float orbitAngle, float orbitRadius, Planet* parent);
	void Draw() override;
	void Update(float deltaTime) override;
	void Init() override;

	CB_VS_vertexshader data;

	DirectX::XMMATRIX modelMat = DirectX::XMMatrixIdentity();
	DirectX::XMMATRIX rotationOrbitMat = DirectX::XMMatrixRotationY(0.0f);
	DirectX::XMMATRIX rotationAxisMat = DirectX::XMMatrixRotationY(0.0f);
	DirectX::XMMATRIX localCoordMat = DirectX::XMMatrixIdentity();

	float orbitAngle = 1.0f;
	float axeAngle = 3.0f;
	float orbitR = 0.0f;
	float r = 1.0f;

private:
	Planet* parent;
	Game* game;
};
