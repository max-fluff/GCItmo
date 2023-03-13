#include "Player.h"

#include <iostream>

#include "../Game.h"
#include "Model3D.h"

Player::Player(Game* game, float r, SphereObject* sphere)
{
	this->game = game;
	this->r = r;
	this->sphere = sphere;
}

void Player::Init()
{
	collider.Center = pos;
	collider.Radius = r;
	pos.y = -scale * r;

	for (const auto gc : game->components)
	{
		auto model = dynamic_cast<Model3D*>(gc);
		if (model)
			models.push_back(model);
	}
}

void Player::Draw()
{
	modelMat = scaleMat * spinMat * translationMat;

	sphere->data.worldViewProj = modelMat *
		game->camera->GetViewMatrix() *
		game->camera->GetProjectionMatrix();

	collider.Center = pos;
	for (const auto model : models)
	{
		if (model->isSticked || collider.Intersects(model->collider))
		{
			if (r * scale >= model->collider.Extents.x / 2 && r * scale >= model->collider.Extents.y / 2 && r * scale >=
				model->collider.Extents.z / 2)
			{
				model->playerCoords = spinMat * translationMat;
				if (!model->isSticked)
				{
					model->pos = model->pos - pos;
					model->worldMatrix = DirectX::XMMatrixScaling(model->scale.x, model->scale.y, model->scale.z) *
						DirectX::XMMatrixTranslation(model->pos.x, model->pos.y, model->pos.z) *

						XMMatrixInverse(nullptr, spinMat);
					model->isSticked = true;
					targetScale += 0.2f;
					game->cameraController->targetArm += 0.2f;
				}
			}
		}
	}

	scaleMat = DirectX::XMMatrixScaling(scale, scale, scale);
	collider.Radius = scale * r;
	translationMat = DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z);
}


void Player::Update(float deltaSec)
{
	if (scale < targetScale)
	{
		scale += deltaSec * 0.1f;
	}
	pos.y = +scale * r;
}
