#include "tdScene.h"

td::TowerDefenseScene::TowerDefenseScene(Color color)
	: scene("Tower Defense 0.0.0", color)
{
}

td::TowerDefenseScene::~TowerDefenseScene()
{
}

void td::TowerDefenseScene::PollInputs()
{
}

void td::TowerDefenseScene::Set()
{
	camera.position = Vector3{ 0, 10, 5 };
	camera.target = Vector3{ 0, 0, 0 };
	camera.up = Vector3{ 0, 1, 0 };
	camera.fovy = 45.0f;
	camera.projection = CAMERA_PERSPECTIVE;

}

void td::TowerDefenseScene::Unset()
{
}

void td::TowerDefenseScene::Update(float dt)
{
}

void td::TowerDefenseScene::FixedUpdate(float timeStep){}

void td::TowerDefenseScene::Draw()
{
	BeginMode3D(camera);
	DrawGrid(10, 1.0f);
	EndMode3D();

	DrawText(name.c_str(), 20, GetScreenHeight() - 30, 20, WHITE);
}

void td::TowerDefenseScene::Inspect()
{
}
