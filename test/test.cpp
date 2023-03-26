#include "DisplayWin32.h"
#include "Game.h"
#include "GeometricPrimitive.h"
#include "postprocess.h"
#include "assimp/scene.h"
#include "assimp/cimport.h"
#include "SimpleMath.h"


int main()
{
	LPCWSTR applicationName = L"My3DApp";
	HINSTANCE hInstance = GetModuleHandle(nullptr);
	auto screenWidth = 800;
	auto screenHeight = 800;


	std::shared_ptr<Game> myGame = std::make_shared<Game>(applicationName, hInstance, screenWidth, screenHeight);

	std::shared_ptr<GameComponent> player = std::make_shared<Player>(myGame, 0.5f, 0.2f, 0.0f, 0.0f,
	                                                              L"Textures\\obama.png", nullptr);

	std::shared_ptr<GameComponent> model1 = std::make_shared<Model3D>(myGame, "MyGame/Models/card.fbx",
	                                                                  L"Textures\\card.jpg",
	                                                                  Vector3(-3.0f, 1.0f, 1.0f),
	                                                                  Vector3(0.2f, 0.2f, 0.2f),
	                                                                  Vector4(0.23f, 0.23f, 0.23f, 100.0f));

	std::shared_ptr<GameComponent> model2 = std::make_shared<Model3D>(myGame, "MyGame/Models/can.fbx",
	                                                                  L"Textures\\can.png",
	                                                                  Vector3(-1.0f, 0.1, 2.0f),
	                                                                  Vector3(1.0f, 1.0f, 1.0f),
	                                                                  Vector4(0.23f, 0.23f, 0.23f, 100.0f));

	std::shared_ptr<GameComponent> model3 = std::make_shared<Model3D>(myGame, "MyGame/Models/hammer.fbx",
	                                                                  L"Textures\\hammer.png",
	                                                                  Vector3(-2.5f, 0.2, 0.0f),
	                                                                  Vector3(3.0f, 3.0f, 3.0f),
	                                                                  Vector4(0.23f, 0.23f, 0.23f, 100.0f));

	std::shared_ptr<GameComponent> model4 = std::make_shared<Model3D>(myGame, "MyGame/Models/chair.fbx",
	                                                                  L"Textures\\chair.png",
	                                                                  Vector3(3.0f, 1.0f, 0.5f),
	                                                                  Vector3(0.03f, 0.03f, 0.03f),
	                                                                  Vector4(0.23f, 0.23f, 0.23f, 10.0f));
	
	std::shared_ptr<GameComponent> model7 = std::make_shared<Model3D>(myGame, "MyGame/Models/floor.fbx",
	                                                                  L"Textures\\grass.jpg",
	                                                                  Vector3(0.0f, 0.0f, 0.0f),
	                                                                  Vector3(0.07f, 0.07f, 0.07f),
	                                                                  Vector4(0.23f, 0.23f, 0.23f, 100.0f));

	myGame->AddGameComponent(player);
	myGame->AddGameComponent(model1);
	myGame->AddGameComponent(model2);
	myGame->AddGameComponent(model3);
	myGame->AddGameComponent(model4);
	myGame->AddGameComponent(model7);


	myGame->dirLight = std::make_shared<DirectionalLight>();
	myGame->dirLight->ambient = Vector4(0.1f, 0.1f, 0.2f, 1.0f);
	myGame->dirLight->direction = Vector4(1.0f, -1.0f, 0.0f, 0.0f);
	myGame->dirLight->light = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	myGame->dirLight->specular = Vector4(0.4f, 0.4f, 0.4f, 50.0f);
	myGame->dirLight->pos = Vector4(-5.0f, 5.0f, 5.0f, 0.0f);
	myGame->InitializeDirectX();

	myGame->StartGameLoop();
}
