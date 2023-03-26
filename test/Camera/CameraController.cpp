#include "CameraController.h"

#include <corecrt_math_defines.h>

#include "../Game.h"

CameraController::CameraController(Player* player)
{
    this->player=player;
}

CameraController::CameraController(Camera* cam, Game* _game,Player* player)
{
    this->game=_game;
    this->camera=cam;
    this->player=player;
    game->wInput->RawOffsetEvent.AddRaw(this,&CameraController::RawInput);
}

void CameraController::RawInput(POINT p)
{
    
    if (game->wInput->IsKeyDown(Keys::RightButton)&&index==-1)
        this->camera->AdjustRotation(p.y*game->deltaTime,-p.x*game->deltaTime,0.0f);
    camera->rotationRollMat*=DirectX::XMMatrixRotationRollPitchYaw(p.y*game->deltaTime,0,0.0f);
    camera->rotationPitchMat*=DirectX::XMMatrixRotationRollPitchYaw(0,-p.x*game->deltaTime,0);

    camera->rot.x+=p.y*game->deltaTime;
    camera->rot.y+=-p.x*game->deltaTime;
        
   

    std::cout<<camera->pos.x<<" "<<camera->pos.y<<" "<<camera->pos.z<<std::endl;

}


void CameraController::CameraMovement(float deltaSec)
{
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
    if (game->wInput->IsKeyDown(Keys::Q))
    {
        SimpleMath::Vector3 upVector = XMVector3Transform(player->UP_VECTOR, camera->rotationPitchMat);
        player->spinMat *= DirectX::XMMatrixRotationRollPitchYaw(-upVector.x * deltaSec, -upVector.y * deltaSec,
            upVector.z * deltaSec);

        player->translationMat = XMMatrixTranslation(player->pos.x, player->pos.y, player->pos.z);
    }
    if (game->wInput->IsKeyDown(Keys::E))
    {
        SimpleMath::Vector3 upVector = XMVector3Transform(player->UP_VECTOR, camera->rotationPitchMat);
        player->spinMat *= DirectX::XMMatrixRotationRollPitchYaw(-upVector.x * deltaSec, upVector.y * deltaSec,
            upVector.z * deltaSec);

        player->translationMat = XMMatrixTranslation(player->pos.x, player->pos.y, player->pos.z);
    }
    if (game->wInput->IsKeyDown(Keys::Space))
    {
        game->dirLight->direction.x = (Vector3(player->pos) - Vector3(camera->pos)).x;
        game->dirLight->direction.y = (Vector3(player->pos) - Vector3(camera->pos)).y;
        game->dirLight->direction.z = (Vector3(player->pos) - Vector3(camera->pos)).z;
        game->dirLight->direction.w = 0.0f;
        game->dirLight->direction.Normalize();
        //camera->pos=player->pos;
    }


    this->camera->pos.x = player->pos.x + sin(camera->rot.y) * cos(camera->rot.x) * arm;
    this->camera->pos.y = player->pos.y - sin(camera->rot.x) * arm;
    this->camera->pos.z = player->pos.z - cos(camera->rot.y) * cos(camera->rot.x) * arm;

    this->camera->SetViewMatrix(XMMatrixIdentity());
    
    this->camera->AddjustTransformation(
        XMMatrixInverse(nullptr, player->translationMat));

    this->player->rotationMat = XMMatrixInverse(nullptr, camera->rotationPitchMat);

    this->camera->AddjustTransformation(camera->rotationPitchMat);
    this->camera->AddjustTransformation(camera->rotationRollMat);
    this->camera->AddjustTransformation(DirectX::XMMatrixTranslation(0, 0, arm));

    if (arm < targetArm)
        arm += deltaSec * 0.1f;
}
