#include "Planet.h"

#include "../Game.h"

class Game;

Planet::Planet(Game* game, float _r, float _axisAngle = 3.0f, float _orbitAngle = 0.5f,
               float _orbitR = 0.0f, LPCWSTR _filepath = L"", Planet* parent = nullptr)
{
	this->game = game;
	this->axeAngle = _axisAngle;
	this->orbitAngle = _orbitAngle;
	this->filePath = _filepath;
	this->orbitR = _orbitR;
	this->r = _r;
	this->parent = parent;
}

void Planet::Draw()
{
	modelMat = rotationAxisMat * DirectX::XMMatrixTranslation(orbitR, 0.0f, 0.0f) * rotationOrbitMat;

	data.worldViewProj = modelMat * localCoordMat *
		game->camera->GetViewMatrix() *
		game->camera->GetProjectionMatrix();
}

void Planet::Update(float deltaTime)
{
	rotationAxisMat *= DirectX::XMMatrixRotationY(axeAngle * 0.01f);
	rotationOrbitMat *= DirectX::XMMatrixRotationY(orbitAngle * 0.01f);

	if (parent)
		localCoordMat = DirectX::XMMatrixTranslation(parent->orbitR, 0.0f, 0) * parent->rotationOrbitMat;
}

void Planet::Init()
{
}
