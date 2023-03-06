#include "CameraController.h"
#include "Game.h"
#include <corecrt_math_defines.h>

CameraController::CameraController()
{
}

CameraController::CameraController(Camera* cam, Game* game)
{
	this->game = game;
	this->camera = cam;
	game->wInput->RawOffsetEvent.AddRaw(this, &CameraController::RawInput);
}

void CameraController::SetPlanetToLookAt(Planet* planetToLookAt)
{
	planet = planetToLookAt;
}

void CameraController::RawInput(POINT p)
{
	if (game->wInput->IsKeyDown(Keys::RightButton))
		this->camera->AdjustRotation(p.y * lastDeltaTime, p.x * lastDeltaTime, 0.0f);

	orbitRotMat*=DirectX::XMMatrixRotationRollPitchYaw(p.y*lastDeltaTime,p.x*lastDeltaTime, 0.0f);
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
	if (game->wInput->IsKeyDown(Keys::LeftControl))
	{
		this->camera->AdjustPosition(0.0f, -cameraSpeed * deltaSec, 0.0f);
	}

	if (!(planet == nullptr)) {

		XMVECTOR determinant;
		camera->SetPosition(0,0,0);
		camera->SetRotation(0,0,0);

		camera->AdjustTransformation(XMMatrixInverse(nullptr, planet->localCoordMat));
		camera->AdjustTransformation(XMMatrixInverse(nullptr, planet->rotationOrbitMat));
		camera->AdjustTransformation(XMMatrixInverse(nullptr, XMMatrixTranslation(planet->orbitR, 0.0f,0.0f)));
		camera->AdjustTransformation(XMMatrixInverse(nullptr, planet->rotationAxisMat));
		//camera->AdjustTransformation(XMMatrixInverse(nullptr, XMMatrixRotationY(planet->axeAngle)));

		camera->AdjustTransformation(orbitRotMat);

		camera->AdjustTransformation(XMMatrixInverse(nullptr, XMMatrixTranslation(planet->r/2.0f+0.5f,0.0f,0.0f)));
		camera->AdjustTransformation(XMMatrixInverse(nullptr, XMMatrixRotationY(-M_PI/2.0f)));
	}
	else 
	{
		orbitRotMat = XMMatrixIdentity();
	}
}


