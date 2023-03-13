#pragma once
#include "Camera.h"
#include "DisplayWin32.h"
#include "DXSDK/Audio.h"
#include "GameComponents/Player.h"

class Game;

class CameraController
{
public:
    CameraController(Player* player);
    CameraController(Camera* cam,Game* game, Player* player);

    void RawInput(POINT p);

    void CameraMovement (float deltaSec);

    Game* game;
    Camera* camera;
    Player* player;
    float cameraSpeed = 2.0f;
    float targetArm = 2.0f;
private:
    float lastDeltaTime;
    float arm = 2.0f;
};
