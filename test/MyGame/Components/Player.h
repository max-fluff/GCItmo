#pragma once

#include <windows.h>
#include <wrl.h>
#include "GeometricPrimitive.h"
#include <DirectXMath.h>
#include "DirectXCollision.h"
#include "SimpleMath.h"
#include "../Shaders/SphereShader.h"

#include "../ConstantBufferTypes.h"
#include "../../RenderComponents/GameComponent.h"


class DepthShader;
class Material;
class Model3D;
class Game;


class Player : public GameComponent
{
public:
	Player(std::shared_ptr<Game> g,float radius,float _axisAngle,float _orbitAngle,float orbitR,LPCWSTR filepath,Player* parent);
	void Initialize() override;
	void Render() override;
	void Update(float deltaSec) override;
	void MainPass() override;
	void DepthPass() override;

	CB_ModelLightning data;

	DirectX::XMMATRIX modelMat= DirectX::XMMatrixIdentity();
	DirectX::XMMATRIX rotationMat=DirectX::XMMatrixIdentity();
	DirectX::XMMATRIX spinMat=DirectX::XMMatrixIdentity();
	DirectX::XMMATRIX translationMat = DirectX::XMMatrixIdentity();
	DirectX::XMMATRIX scaleMat = DirectX::XMMatrixIdentity();
	DirectX::XMMATRIX localCoordMat =DirectX::XMMatrixIdentity();

	DirectX::SimpleMath::Vector3 pos=DirectX::XMFLOAT3(0,0,0);
	DirectX::SimpleMath::Vector3 rot=DirectX::XMFLOAT3(0,0,0);

	DirectX::SimpleMath::Vector3 FORWARD_VECTOR=DirectX::XMFLOAT3(0.0f,0.0f,1.0f);
	DirectX::SimpleMath::Vector3 LEFT_VECTOR=DirectX::XMFLOAT3(1.0f,0.0f,0.0f);
	DirectX::SimpleMath::Vector3 UP_VECTOR = DirectX::XMFLOAT3(0.0f, 2.0f, 0.0f);

	std::shared_ptr<Material> material;
	float axeAngle=0;
	float orbitAngle=0;
	float orbitR=0;
	float r=0;
	float scale=1.0f;
	float targetScale=1.0f;
	LPCWSTR filePath;
	std::shared_ptr<SphereShader> shader;
	DepthShader* depthShader;
	DirectX::BoundingSphere collider;

	std::vector<Model3D*> models;
	Model3D* sphereModel;
private:
	
	
	Player* parent;

};

