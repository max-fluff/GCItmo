#pragma once
#include "Camera.h"
#include "DisplayWin32.h"
#include "GameComponents/Planet.h"

class Game;

class CameraController
{
public:
    CameraController();
    CameraController(Camera* cam,Game* game );

    void SetPlanetToLookAt(Planet* planetToLookAt);

    void RawInput(POINT p);

    void CameraMovement (float deltaSec);

    Game* game;
    Camera* camera;
    float cameraSpeed = 2.0f;
private:
    float lastDeltaTime;
    Planet* planet;

    DirectX::XMMATRIX orbitRotMat;
};
