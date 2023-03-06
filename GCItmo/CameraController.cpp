#include "CameraController.h"
#include "Game.h"

CameraController::CameraController()
{
}

CameraController::CameraController(Camera* cam, Game* game)
{
	this->game = game;
	this->camera = cam;
	game->wInput->RawOffsetEvent.AddRaw(this, &CameraController::RawInput);
}

void CameraController::RawInput(POINT p)
{
	if (game->wInput->IsKeyDown(Keys::RightButton))
		this->camera->AdjustRotation(p.y * lastDeltaTime, p.x * lastDeltaTime, 0.0f);
}

void CameraController::CameraMovement(float deltaSec)
{
	lastDeltaTime = deltaSec;

	using namespace DirectX;

	if (game->wInput->IsKeyDown(Keys::W))
	{
		this->camera->AdjustPosition(this->camera->GetForwardVector() * cameraSpeed * deltaSec);
	}
	if (game->wInput->IsKeyDown(Keys::A))
	{
		this->camera->AdjustPosition(this->camera->GetLeftVector() * cameraSpeed * deltaSec);
	}
	if (game->wInput->IsKeyDown(Keys::S))
	{
		this->camera->AdjustPosition(this->camera->GetBackwardVector() * cameraSpeed * deltaSec);
	}
	if (game->wInput->IsKeyDown(Keys::D))
	{
		this->camera->AdjustPosition(this->camera->GetRightVector() * cameraSpeed * deltaSec);
	}
	if (game->wInput->IsKeyDown(Keys::Space))
	{
		this->camera->AdjustPosition(0.0f, cameraSpeed * deltaSec, 0.0f);
	}
	if (game->wInput->IsKeyDown(Keys::LeftShift))
	{
		this->camera->AdjustPosition(0.0f, -cameraSpeed * deltaSec, 0.0f);
	}
}
