#pragma once

#include <DirectXMath.h>
#include <memory>

#include "DirectXCollision.h"
#include "SphereObject.h"

#include "../GameComponent.h"
#include "../DXSDK/SimpleMath.h"

class StickyModel;
class Game;

class Player : public GameComponent
{
public:
	Player(Game* g,float radius, SphereObject* sphere);
	void Init() override;
	void Draw() override;
	void Update(float deltaSec) override;

	DirectX::XMMATRIX modelMat= DirectX::XMMatrixIdentity();
	DirectX::XMMATRIX rotationMat=DirectX::XMMatrixIdentity();
	DirectX::XMMATRIX spinMat=DirectX::XMMatrixIdentity();
	DirectX::XMMATRIX translationMat = DirectX::XMMatrixIdentity();
	DirectX::XMMATRIX scaleMat = DirectX::XMMatrixIdentity();
	DirectX::XMMATRIX localCoordMat =DirectX::XMMatrixIdentity();

	DirectX::SimpleMath::Vector3 pos=DirectX::XMFLOAT3(0,0,0);
	DirectX::SimpleMath::Vector3 rot=DirectX::XMFLOAT3(0,0,0);

	DirectX::SimpleMath::Vector3 FORWARD_VECTOR=DirectX::XMFLOAT3(0.0f,0.0f,2.0f);
	DirectX::SimpleMath::Vector3 LEFT_VECTOR=DirectX::XMFLOAT3(2.0f,0.0f,0.0f);
	DirectX::SimpleMath::Vector3 UP_VECTOR = DirectX::XMFLOAT3(0.0f, 2.0f, 0.0f);

	float r=0;
	float scale=1.0f;
	float targetScale=1.0f;

	std::shared_ptr<SphereObject> shader;
	DirectX::BoundingSphere collider;

	std::vector<StickyModel*> models;
private:
	Game* game;
	SphereObject* sphere;
};

