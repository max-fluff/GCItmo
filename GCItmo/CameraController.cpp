#include "CameraController.h"

#include <corecrt_math_defines.h>

#include "Game.h"
#include "DXSDK/SimpleMath.h"
#include "GameComponents/Player.h"

CameraController::CameraController(Player* player)
{
	this->player = player;
}

CameraController::CameraController(Camera* cam, Game* _game, Player* player)
{
	this->game = _game;
	this->camera = cam;
	this->player = player;
	game->wInput->RawOffsetEvent.AddRaw(this, &CameraController::RawInput);
}

void CameraController::RawInput(POINT p)
{
	if (game->wInput->IsKeyDown(Keys::RightButton))
		this->camera->AdjustRotation(p.y * lastDeltaTime, -p.x * lastDeltaTime, 0.0f);
	camera->rotationRollMat *= DirectX::XMMatrixRotationRollPitchYaw(p.y * lastDeltaTime, 0, 0.0f);
	camera->rotationPitchMat *= DirectX::XMMatrixRotationRollPitchYaw(0, -p.x * lastDeltaTime, 0);
}


void CameraController::CameraMovement(float deltaSec)
{
	lastDeltaTime = deltaSec;
	using namespace DirectX;
	if (game->wInput->IsKeyDown(Keys::W))
	{
		SimpleMath::Vector3 frwdVector = XMVector3Transform(player->FORWARD_VECTOR, camera->rotationPitchMat);
		SimpleMath::Vector3 lftVector = XMVector3Transform(player->LEFT_VECTOR, camera->rotationPitchMat);
		player->spinMat *= DirectX::XMMatrixRotationRollPitchYaw(lftVector.x * deltaSec, -lftVector.y * deltaSec,
		                                                         -lftVector.z * deltaSec);
		player->pos += SimpleMath::Vector3(-frwdVector.x, 0, frwdVector.z) * deltaSec;

		player->translationMat = XMMatrixTranslation(player->pos.x, player->pos.y, player->pos.z);
	}
	if (game->wInput->IsKeyDown(Keys::A))
	{
		SimpleMath::Vector3 frwdVector = XMVector3Transform(player->FORWARD_VECTOR, camera->rotationPitchMat);
		SimpleMath::Vector3 lftVector = XMVector3Transform(player->LEFT_VECTOR, camera->rotationPitchMat);
		player->spinMat *= DirectX::XMMatrixRotationRollPitchYaw(-frwdVector.x * deltaSec, -frwdVector.y * deltaSec,
			frwdVector.z * deltaSec);
		player->pos += SimpleMath::Vector3(-lftVector.x, 0, lftVector.z) * deltaSec;

		player->translationMat = XMMatrixTranslation(player->pos.x, player->pos.y, player->pos.z);
	}
	if (game->wInput->IsKeyDown(Keys::S))
	{
		SimpleMath::Vector3 frwdVector = XMVector3Transform(player->FORWARD_VECTOR, camera->rotationPitchMat);
		SimpleMath::Vector3 lftVector = XMVector3Transform(player->LEFT_VECTOR, camera->rotationPitchMat);

		player->spinMat *= DirectX::XMMatrixRotationRollPitchYaw(-lftVector.x * deltaSec, -lftVector.y * deltaSec,
			lftVector.z * deltaSec);
		player->pos -= SimpleMath::Vector3(-frwdVector.x, 0, frwdVector.z) * deltaSec;

		player->translationMat = XMMatrixTranslation(player->pos.x, player->pos.y, player->pos.z);
	}
	if (game->wInput->IsKeyDown(Keys::D))
	{
				SimpleMath::Vector3 frwdVector = XMVector3Transform(player->FORWARD_VECTOR, camera->rotationPitchMat);

		SimpleMath::Vector3 lftVector = XMVector3Transform(player->LEFT_VECTOR, camera->rotationPitchMat);
		player->spinMat *= DirectX::XMMatrixRotationRollPitchYaw(frwdVector.x * deltaSec, -frwdVector.y * deltaSec,
			-frwdVector.z * deltaSec);
		player->pos -= SimpleMath::Vector3(-lftVector.x, 0, lftVector.z) * deltaSec;

		player->translationMat = XMMatrixTranslation(player->pos.x, player->pos.y, player->pos.z);
	}
	if (game->wInput->IsKeyDown(Keys::Space))
	{
		this->camera->AdjustPosition(0.0f, cameraSpeed * deltaSec, 0.0f);
	}
	if (game->wInput->IsKeyDown(Keys::LeftShift))
	{
		this->camera->AdjustPosition(0.0f, -cameraSpeed * deltaSec, 0.0f);
	}


	this->camera->SetViewMatrix(XMMatrixIdentity());
	this->camera->SetPosition(0, 0, 0);
	this->camera->SetRotation(0, 0, 0);


	//Переходим в координаты шара
	this->camera->AdjustTransformation(XMMatrixInverse(nullptr, player->translationMat));

	this->player->rotationMat = XMMatrixInverse(nullptr, camera->rotationPitchMat);

	this->camera->AdjustTransformation(camera->rotationPitchMat);
	this->camera->AdjustTransformation(camera->rotationRollMat);
	this->camera->AdjustTransformation(DirectX::XMMatrixTranslation(0, 0, arm));


	if (arm < targetArm)
		arm += deltaSec * 0.1f;
}
