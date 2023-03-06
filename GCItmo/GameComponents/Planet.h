#pragma once

#include "../GameComponent.h"
#include "../DXSDK/ConstantBufferTypes.h"
#include "../GameComponents/SphereObject.h"
#include "../DXSDK/SimpleMath.h"

class Game;

class Planet : public GameComponent
{
public:
	Planet(Game* game, float radius, float axisAngle, float orbitAngle, float orbitRadius, DirectX::SimpleMath::Vector3 axisVector, Planet* parent);
	void Draw() override;
	void Update(float deltaTime) override;
	void Init() override;

	CB_VS_vertexshader data;

	DirectX::SimpleMath::Matrix modelMat = DirectX::XMMatrixIdentity();
	DirectX::SimpleMath::Matrix rotationOrbitMat = DirectX::XMMatrixRotationY(0.0f);
	DirectX::SimpleMath::Matrix rotationAxisMat = DirectX::XMMatrixRotationY(0.0f);
	DirectX::SimpleMath::Matrix localCoordMat = DirectX::XMMatrixIdentity();

	float orbitAngle = 1.0f;
	float axeAngle = 3.0f;
	float orbitR = 0.0f;
	float r = 1.0f;

private:
	Planet* parent;
	Game* game;

	DirectX::SimpleMath::Vector3 axisVector;
};
