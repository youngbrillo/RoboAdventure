
#pragma once
#include "test.h"
#include <gamelib/core/components/towerDefenseComponents.h>
// #include "../../components/towerDefense/common.h"

namespace glib::towerdefense
{

	GUIState guiState = { 0 };
	GameTime gameTime = { 0 };
	Level* currentLevel = nullptr;
	class TowerDefenseTest : public testbed::Test
	{
		Camera3D camera;

		std::vector<Level> levels;
		float nextSpawnTime = 0.0f;

	public:
		TowerDefenseTest()
			: Test("Tower Defense", DARKBLUE)
		{
			camera.position = { 10, 10, 10 };
			camera.target = { 0,0,0 };
			camera.up = { 0, 1, 0 };
			camera.fovy = 45;
			camera.projection = CAMERA_PERSPECTIVE;

			levels = {
				{
					.state = LEVEL_STATE_BUILDING,
					.initialGold = 20,
					.waves = {
						{
						  .enemyType = ENEMY_TYPE_MINION,
						  .wave = 0,
						  .count = 10,
						  .interval = 2.5f,
						  .delay = 1.0f,
						  .spawnPosition = {0, 6},
						},
						{
						  .enemyType = ENEMY_TYPE_MINION,
						  .wave = 1,
						  .count = 20,
						  .interval = 1.5f,
						  .delay = 1.0f,
						  .spawnPosition = {0, 6},
						},
						{
						  .enemyType = ENEMY_TYPE_MINION,
						  .wave = 2,
						  .count = 30,
						  .interval = 1.2f,
						  .delay = 1.0f,
						  .spawnPosition = {0, 6},
						}
					}
				}
			};

		}

		~TowerDefenseTest()
		{

		}

		virtual void Set()
		{
			currentLevel = &levels[0];
			nextSpawnTime = 0.0f;
			InitGame();

		}

		void InitLevel(Level* level)
		{
			TowerInit();
			EnemyInit();
			ProjectileInit();
			ParticleInit();
			TowerTryAdd(TOWER_TYPE_BASE, 0, 0);

			level->placementMode = 0;
			level->state = LEVEL_STATE_BUILDING;
			level->nextState = LEVEL_STATE_NONE;
			level->playerGold = level->initialGold;

			Camera* camera = &level->camera;
			camera->position = Vector3{ 4.0f, 8.0f, 8.0f };
			camera->target = Vector3{ 0.0f, 0.0f, 0.0f };
			camera->up = Vector3{ 0.0f, 1.0f, 0.0f };
			camera->fovy = 10.0f;
			camera->projection = CAMERA_ORTHOGRAPHIC;
		}


		void DrawLevelHud(Level* level)
		{
			const char* text = TextFormat("Gold: %d", level->playerGold);
			Font font = GetFontDefault();
			DrawTextEx(font, text, Vector2 { GetScreenWidth() - 120.0f, 10+30 }, font.baseSize * 2.0f, 2.0f, BLACK);
			DrawTextEx(font, text, Vector2 { GetScreenWidth() - 122.0f, 8+30 }, font.baseSize * 2.0f, 2.0f, YELLOW);
		}

		void DrawLevelReportLostWave(Level* level)
		{
			BeginMode3D(level->camera);
			DrawGrid(10, 1.0f);
			TowerDraw();
			EnemyDraw();
			ProjectileDraw();
			ParticleDraw();
			guiState.isBlocked = 0;
			EndMode3D();

			TowerDrawHealthBars(level->camera);

			const char* text = "Wave lost";
			int textWidth = MeasureText(text, 20);
			DrawText(text, (GetScreenWidth() - textWidth) * 0.5f, 20, 20, WHITE);

			if (Button("Reset level", 20, GetScreenHeight() - 40, 160, 30, 0))
			{
				level->nextState = LEVEL_STATE_RESET;
			}
		}

		int HasLevelNextWave(Level* level)
		{
			for (int i = 0; i < 10; i++)
			{
				EnemyWave* wave = &level->waves[i];
				if (wave->wave == level->currentWave)
				{
					return 1;
				}
			}
			return 0;
		}

		void DrawLevelReportWonWave(Level* level)
		{
			BeginMode3D(level->camera);
			DrawGrid(10, 1.0f);
			TowerDraw();
			EnemyDraw();
			ProjectileDraw();
			ParticleDraw();
			guiState.isBlocked = 0;
			EndMode3D();

			TowerDrawHealthBars(level->camera);

			const char* text = "Wave won";
			int textWidth = MeasureText(text, 20);
			DrawText(text, (GetScreenWidth() - textWidth) * 0.5f, 20, 20, WHITE);

			ButtonState state = { 0 };

			if (Button("Reset level", 20, GetScreenHeight() - 40, 160, 30, 0))
			{
				level->nextState = LEVEL_STATE_RESET;
			}

			if (HasLevelNextWave(level))
			{
				if (Button("Prepare for next wave", GetScreenWidth() - 300, GetScreenHeight() - 40, 300, 30, &state))
				{
					level->nextState = LEVEL_STATE_BUILDING;
				}
			}
			else {
				if (Button("Level won", GetScreenWidth() - 300, GetScreenHeight() - 40, 300, 30, &state))
				{
					level->nextState = LEVEL_STATE_WON_LEVEL;
				}
			}
		}

		void DrawBuildingBuildButton(Level* level, int x, int y, int width, int height, uint8_t towerType, const char* name)
		{
			static ButtonState buttonStates[8] = { 0 };
			int cost = GetTowerCosts(towerType);
			const char* text = TextFormat("%s: %d", name, cost);
			buttonStates[towerType].isSelected = level->placementMode == towerType;
			buttonStates[towerType].isDisabled = level->playerGold < cost;
			if (Button(text, x, y, width, height, &buttonStates[towerType]))
			{
				level->placementMode = buttonStates[towerType].isSelected ? 0 : towerType;
			}
		}

		void DrawLevelBuildingState(Level* level)
		{
			BeginMode3D(level->camera);
			DrawGrid(10, 1.0f);
			TowerDraw();
			EnemyDraw();
			ProjectileDraw();
			ParticleDraw();

			Ray ray = GetScreenToWorldRay(GetMousePosition(), level->camera);
			float planeDistance = ray.position.y / -ray.direction.y;
			float planeX = ray.direction.x * planeDistance + ray.position.x;
			float planeY = ray.direction.z * planeDistance + ray.position.z;
			int16_t mapX = (int16_t)floorf(planeX + 0.5f);
			int16_t mapY = (int16_t)floorf(planeY + 0.5f);
			if (level->placementMode && !guiState.isBlocked)
			{
				DrawCubeWires(Vector3 { (float)mapX, 0.2f, (float)mapY }, 1.0f, 0.4f, 1.0f, RED);
				if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
				{
					if (TowerTryAdd(level->placementMode, mapX, mapY))
					{
						level->playerGold -= GetTowerCosts(level->placementMode);
						level->placementMode = TOWER_TYPE_NONE;
					}
				}
			}

			guiState.isBlocked = 0;

			EndMode3D();

			TowerDrawHealthBars(level->camera);

			static ButtonState buildWallButtonState = { 0 };
			static ButtonState buildGunButtonState = { 0 };
			buildWallButtonState.isSelected = level->placementMode == TOWER_TYPE_WALL;
			buildGunButtonState.isSelected = level->placementMode == TOWER_TYPE_GUN;

			DrawBuildingBuildButton(level, 10, 10+30, 80, 30, TOWER_TYPE_WALL, "Wall");
			DrawBuildingBuildButton(level, 10, 50+30, 80, 30, TOWER_TYPE_GUN, "Gun");

			ButtonState state = { 0 };

			if (Button("Reset level", 20, GetScreenHeight() - 40, 160, 30, &state))
			{
				level->nextState = LEVEL_STATE_RESET;
			}
			if (Button("Begin waves", GetScreenWidth() - 160, GetScreenHeight() - 40, 160, 30, &state))
			{
				level->nextState = LEVEL_STATE_BATTLE;
			}

			const char* text = "Building phase";
			int textWidth = MeasureText(text, 20);
			DrawText(text, (GetScreenWidth() - textWidth) * 0.5f, 20+30, 20, WHITE);
		}

		void InitBattleStateConditions(Level* level)
		{
			level->state = LEVEL_STATE_BATTLE;
			level->nextState = LEVEL_STATE_NONE;
			level->waveEndTimer = 0.0f;
			for (int i = 0; i < 10; i++)
			{
				EnemyWave* wave = &level->waves[i];
				wave->spawned = 0;
				wave->timeToSpawnNext = wave->delay;
			}
		}

		void DrawLevelBattleState(Level* level)
		{
			BeginMode3D(level->camera);
			DrawGrid(10, 1.0f);
			TowerDraw();
			EnemyDraw();
			ProjectileDraw();
			ParticleDraw();
			guiState.isBlocked = 0;
			EndMode3D();

			EnemyDrawHealthbars(level->camera);
			TowerDrawHealthBars(level->camera);

			if (Button("Reset level", 20, GetScreenHeight() - 40, 160, 30, 0))
			{
				level->nextState = LEVEL_STATE_RESET;
			}

			int maxCount = 0;
			int remainingCount = 0;
			for (int i = 0; i < 10; i++)
			{
				EnemyWave* wave = &level->waves[i];
				if (wave->wave != level->currentWave)
				{
					continue;
				}
				maxCount += wave->count;
				remainingCount += wave->count - wave->spawned;
			}
			int aliveCount = EnemyCount();
			remainingCount += aliveCount;

			const char* text = TextFormat("Battle phase: %03d%%", 100 - remainingCount * 100 / maxCount);
			int textWidth = MeasureText(text, 20);
			DrawText(text, (GetScreenWidth() - textWidth) * 0.5f, 20, 20, WHITE);
		}

		void DrawLevel(Level* level)
		{
			switch (level->state)
			{
			case LEVEL_STATE_BUILDING: DrawLevelBuildingState(level); break;
			case LEVEL_STATE_BATTLE: DrawLevelBattleState(level); break;
			case LEVEL_STATE_WON_WAVE: DrawLevelReportWonWave(level); break;
			case LEVEL_STATE_LOST_WAVE: DrawLevelReportLostWave(level); break;
			default: break;
			}

			DrawLevelHud(level);
		}

		void UpdateLevel(Level* level)
		{
			if (level->state == LEVEL_STATE_BATTLE)
			{
				int activeWaves = 0;
				for (int i = 0; i < 10; i++)
				{
					EnemyWave* wave = &level->waves[i];
					if (wave->spawned >= wave->count || wave->wave != level->currentWave)
					{
						continue;
					}
					activeWaves++;
					wave->timeToSpawnNext -= gameTime.deltaTime;
					if (wave->timeToSpawnNext <= 0.0f)
					{
						Enemy* enemy = EnemyTryAdd(wave->enemyType, wave->spawnPosition.x, wave->spawnPosition.y);
						if (enemy)
						{
							wave->timeToSpawnNext = wave->interval;
							wave->spawned++;
						}
					}
				}
				if (GetTowerByType(TOWER_TYPE_BASE) == 0) {
					level->waveEndTimer += gameTime.deltaTime;
					if (level->waveEndTimer >= 2.0f)
					{
						level->nextState = LEVEL_STATE_LOST_WAVE;
					}
				}
				else if (activeWaves == 0 && EnemyCount() == 0)
				{
					level->waveEndTimer += gameTime.deltaTime;
					if (level->waveEndTimer >= 2.0f)
					{
						level->nextState = LEVEL_STATE_WON_WAVE;
					}
				}
			}

			PathFindingMapUpdate();
			EnemyUpdate();
			TowerUpdate();
			ProjectileUpdate();
			ParticleUpdate();

			if (level->nextState == LEVEL_STATE_RESET)
			{
				InitLevel(level);
			}

			if (level->nextState == LEVEL_STATE_BATTLE)
			{
				InitBattleStateConditions(level);
			}

			if (level->nextState == LEVEL_STATE_WON_WAVE)
			{
				level->currentWave++;
				level->state = LEVEL_STATE_WON_WAVE;
			}

			if (level->nextState == LEVEL_STATE_LOST_WAVE)
			{
				level->state = LEVEL_STATE_LOST_WAVE;
			}

			if (level->nextState == LEVEL_STATE_BUILDING)
			{
				level->state = LEVEL_STATE_BUILDING;
			}

			if (level->nextState == LEVEL_STATE_WON_LEVEL)
			{
				// make something of this later
				InitLevel(level);
			}

			level->nextState = LEVEL_STATE_NONE;
		}


		//# Immediate GUI functions

		//# Main game loop

		void GameUpdate()
		{
			float dt = GetFrameTime();
			// cap maximum delta time to 0.1 seconds to prevent large time steps
			if (dt > 0.1f) dt = 0.1f;
			gameTime.time += dt;
			gameTime.deltaTime = dt;

			UpdateLevel(currentLevel);
		}


		void InitGame()
		{
			TowerInit();
			EnemyInit();
			ProjectileInit();
			ParticleInit();
			PathfindingMapInit(20, 20, Vector3 { -10.0f, 0.0f, -10.0f }, 1.0f);

			currentLevel = &levels[0];
			InitLevel(currentLevel);
		}

		virtual void Unset()
		{

		}

		virtual void Update(float dt)
		{
			GameUpdate();
		}
		virtual void Draw()
		{
			//BeginMode3D(camera);
			//DrawGrid(10, 1.0f);
			//EndMode3D();
			DrawLevel(currentLevel);
		};
		virtual void Inspect()
		{

		};
		static Test* Generate() { return new TowerDefenseTest(); }
	};

	static int entry = testbed::TestManager::AddEntry("Tower Defense", "tower defense", glib::towerdefense::TowerDefenseTest::Generate);
}