#include "tdScene.h"

td::TowerDefenseScene::TowerDefenseScene(Color color)
	: scene("Tower Defense 0.0.0", color)
	, towers(ecs)
	, enemies(ecs)
	, projectiles(ecs)
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
	camera.position = Vector3{ 10, 10, 10 };
	camera.target = Vector3{ 0, 0, 0 };
	camera.up = Vector3{ 0, 1, 0 };
	camera.fovy = 45.0f;
	camera.projection = CAMERA_PERSPECTIVE;

	//init towers
	towers.Init();
	enemies.Init();
	projectiles.Init();

	towers.TryAdd(TOWER_TYPE_BASE, iVec2{ 0, 0 });
	towers.TryAdd(TOWER_TYPE_GUN, iVec2{ 2, 0 })->cooldown.duration = 0.5f;
	towers.TryAdd(TOWER_TYPE_GUN, iVec2{ -2, 0 })->cooldown.duration = 0.2f;

	enemies.TryAdd(ENEMY_TYPE_MINION, iVec2{ 5, 4 });

	entt::entity g_t = ecs.create();
	ecs.emplace<GameTime>(g_t);
}

void td::TowerDefenseScene::Unset()
{
	ecs.clear();
}

void td::TowerDefenseScene::Update(float dt)
{
	if (dt > 0.1f) dt = 0.1f;

	if (GameTime* gt = GetGameTime(ecs))
	{
		gt->time += dt;
		gt->deltaTime = dt;
	}
	enemies.Update(dt);
	towers.Update(enemies, projectiles, dt);
	projectiles.Update(enemies, dt);
}

void td::TowerDefenseScene::FixedUpdate(float timeStep){}

void td::TowerDefenseScene::Draw()
{
	BeginMode3D(camera);
		DrawGrid(10, 1.0f);
		towers.Draw();
		enemies.Draw();
		projectiles.Draw();
	EndMode3D();

	DrawText(name.c_str(), 20, GetScreenHeight() - 30, 20, WHITE);
}

void td::TowerDefenseScene::Inspect()
{
}
