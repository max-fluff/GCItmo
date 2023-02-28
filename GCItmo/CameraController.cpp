#include "CameraController.h"
#include "Game.h"
#include "Keys.h"

CameraController::CameraController()
{
    
}

CameraController::CameraController(Camera* cam, Game* _game)
{
    this->game=_game;
    this->camera=cam;
    game->inputDevice->RawOffsetEvent.AddRaw(this,&CameraController::RawInput);
}

void CameraController::RawInput(POINT p)
{
    if (game->inputDevice->IsKeyDown(Keys::RightButton))
        this->camera->AdjustRotation(p.y*0.01f,p.x*0.01f,0.0f);
}


void CameraController::CameraMovement(float deltaSec)
{
    using namespace DirectX;
    int index=-1;
    if (game->inputDevice->IsKeyDown(Keys::W))
    {
        this->camera->AdjustPosition(this->camera->GetForwardVector()*cameraSpeed*deltaSec);
    }
    if (game->inputDevice->IsKeyDown(Keys::A))
    {
        this->camera->AdjustPosition(this->camera->GetLeftVector()*cameraSpeed*deltaSec);
    }
    if (game->inputDevice->IsKeyDown(Keys::S))
    {
        this->camera->AdjustPosition( this->camera->GetBackwardVector()*cameraSpeed*deltaSec);
    }
    if (game->inputDevice->IsKeyDown(Keys::D))
    {
        this->camera->AdjustPosition(this->camera->GetRightVector()*cameraSpeed*deltaSec);
    }
    if (game->inputDevice->IsKeyDown(Keys::Space))
    {
        this->camera->AdjustPosition(0.0f,cameraSpeed*deltaSec,0.0f);
    }
    if (game->inputDevice->IsKeyDown(Keys::LeftShift))
    {
        this->camera->AdjustPosition(0.0f,-cameraSpeed*deltaSec,0.0f);
    }    
}
