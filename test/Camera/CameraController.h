#pragma once
#include <memory>

#include "Camera.h"

#include "../MyGame/Components/Player.h"


#include "../DirectXSDK/Delegates.h"


class Game;
class CameraController
{
public:
    CameraController(Player* player);
    CameraController(Camera* cam,Game* _game,Player* player );

    void RawInput(POINT p);

    void CameraMovement (float deltaSec);
    

    Game* game;
    Camera* camera;
    Player* player;
    float cameraSpeed = 2.0f;
    float arm=2.0f;
    float targetArm=2.0f;
    DirectX::XMMATRIX orbitRotMat=DirectX::XMMatrixIdentity();
    int index=0;
};
