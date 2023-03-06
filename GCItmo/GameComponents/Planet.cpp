#include "Planet.h"

#include "../Game.h"

class Game;

Planet::Planet(Game* game, float radius, float axisAngle = 3.0f, float orbitAngle = 0.5f,
               float orbitRadius = 0.0f, Planet* parent = nullptr)
{
	this->game = game;
	this->axeAngle = axisAngle;
	this->orbitAngle = orbitAngle;
	this->orbitR = orbitRadius;
	this->r = radius;
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
