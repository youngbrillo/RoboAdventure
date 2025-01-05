#pragma once

#include <iostream>
#include <raylib/raylib.h>
#include <raylib/raymath.h>
#include <vector>
#include <string>
#include <entt/entt.hpp>

namespace glib::towerdefense
{

#define ENEMY_MAX_PATH_COUNT 8
#define ENEMY_MAX_COUNT 400
#define ENEMY_TYPE_NONE 0
#define ENEMY_TYPE_MINION 1

#define PARTICLE_MAX_COUNT 400
#define PARTICLE_TYPE_NONE 0
#define PARTICLE_TYPE_EXPLOSION 1

	struct Particle
	{
		uint8_t particleType;
		float spawnTime;
		float lifetime;
		Vector3 position;
		Vector3 velocity;
	};

#define TOWER_MAX_COUNT 400
#define TOWER_TYPE_NONE 0
#define TOWER_TYPE_BASE 1
#define TOWER_TYPE_GUN 2
#define TOWER_TYPE_WALL 3
#define TOWER_TYPE_COUNT 4

	struct Tower
	{
		int16_t x, y;
		uint8_t towerType;
		float cooldown;
		float damage;
	};

	struct GameTime
	{
		float time;
		float deltaTime;
	};

	struct ButtonState {
		char isSelected;
		char isDisabled;
	};

	struct GUIState {
		int isBlocked;
	};

	enum LevelState
	{
		LEVEL_STATE_NONE,
		LEVEL_STATE_BUILDING,
		LEVEL_STATE_BATTLE,
		LEVEL_STATE_WON_WAVE,
		LEVEL_STATE_LOST_WAVE,
		LEVEL_STATE_WON_LEVEL,
		LEVEL_STATE_RESET,
	};

	struct EnemyWave {
		uint8_t enemyType;
		uint8_t wave;
		uint16_t count;
		float interval;
		float delay;
		Vector2 spawnPosition;

		uint16_t spawned;
		float timeToSpawnNext;
	};

	struct Level
	{
		LevelState state;
		LevelState nextState;
		Camera3D camera;
		int placementMode;

		int initialGold;
		int playerGold;

		EnemyWave waves[10];
		int currentWave;
		float waveEndTimer;
	};

	struct DeltaSrc
	{
		char x, y;
	};

	struct PathfindingMap
	{
		int width, height;
		float scale;
		float* distances;
		long* towerIndex;
		DeltaSrc* deltaSrc;
		float maxDistance;
		Matrix toMapSpace;
		Matrix toWorldSpace;
	};

	// when we execute the pathfinding algorithm, we need to store the active nodes
	// in a queue. Each node has a position, a distance from the start, and the
	// position of the node that we came from.
	struct PathfindingNode
	{
		int16_t x, y, fromX, fromY;
		float distance;
	};

	struct EnemyId
	{
		uint16_t index;
		uint16_t generation;
	};

	struct EnemyClassConfig
	{
		float speed;
		float health;
		float radius;
		float maxAcceleration;
		float requiredContactTime;
		float explosionDamage;
		float explosionRange;
		float explosionPushbackPower;
		int goldValue;
	};

	struct Enemy
	{
		int16_t currentX, currentY;
		int16_t nextX, nextY;
		Vector2 simPosition;
		Vector2 simVelocity;
		uint16_t generation;
		float startMovingTime;
		float damage, futureDamage;
		float contactTime;
		uint8_t enemyType;
		uint8_t movePathCount;
		Vector2 movePath[ENEMY_MAX_PATH_COUNT];
	};

#define PROJECTILE_MAX_COUNT 1200
#define PROJECTILE_TYPE_NONE 0
#define PROJECTILE_TYPE_BULLET 1

	struct Projectile
	{
		uint8_t projectileType;
		float shootTime;
		float arrivalTime;
		float damage;
		Vector2 position;
		Vector2 target;
		Vector2 directionNormal;
		EnemyId targetEnemy;
	};

	//# Function declarations
	float TowerGetMaxHealth(Tower* tower);
	bool Button(const char* text, int x, int y, int width, int height, ButtonState* state);
	int EnemyAddDamage(Enemy* enemy, float damage);

	//# Enemy functions
	void EnemyInit();
	void EnemyDraw();
	void EnemyTriggerExplode(Enemy* enemy, Tower* tower, Vector3 explosionSource);
	void EnemyUpdate();
	float EnemyGetCurrentMaxSpeed(Enemy* enemy);
	float EnemyGetMaxHealth(Enemy* enemy);
	int EnemyGetNextPosition(int16_t currentX, int16_t currentY, int16_t* nextX, int16_t* nextY);
	Vector2 EnemyGetPosition(Enemy* enemy, float deltaT, Vector2* velocity, int* waypointPassedCount);
	EnemyId EnemyGetId(Enemy* enemy);
	Enemy* EnemyTryResolve(EnemyId enemyId);
	Enemy* EnemyTryAdd(uint8_t enemyType, int16_t currentX, int16_t currentY);
	int EnemyAddDamage(Enemy* enemy, float damage);
	Enemy* EnemyGetClosestToCastle(int16_t towerX, int16_t towerY, float range);
	int EnemyCount();
	void EnemyDrawHealthbars(Camera3D camera);

	//# Tower functions
	void TowerInit();
	Tower* TowerGetAt(int16_t x, int16_t y);
	Tower* TowerTryAdd(uint8_t towerType, int16_t x, int16_t y);
	Tower* GetTowerByType(uint8_t towerType);
	int GetTowerCosts(uint8_t towerType);
	float TowerGetMaxHealth(Tower* tower);
	void TowerDraw();
	void TowerUpdate();
	void TowerDrawHealthBars(Camera3D camera);

	//# Particles
	void ParticleInit();
	void ParticleAdd(uint8_t particleType, Vector3 position, Vector3 velocity, float lifetime);
	void ParticleUpdate();
	void ParticleDraw();

	//# Projectiles
	void ProjectileInit();
	void ProjectileDraw();
	void ProjectileUpdate();
	Projectile* ProjectileTryAdd(uint8_t projectileType, Enemy* enemy, Vector2 position, Vector2 target, float speed, float damage);

	//# Pathfinding map
	void PathfindingMapInit(int width, int height, Vector3 translate, float scale);
	float PathFindingGetDistance(int mapX, int mapY);
	Vector2 PathFindingGetGradient(Vector3 world);
	int PathFindingFromWorldToMapPosition(Vector3 worldPosition, int16_t* mapX, int16_t* mapY);
	void PathFindingMapUpdate();
	void PathFindingMapDraw();

	//# UI
	void DrawHealthBar(Camera3D camera, Vector3 position, float healthRatio, Color barColor);

	//# variables
	extern Level* currentLevel;
	extern Enemy enemies[ENEMY_MAX_COUNT];
	extern int enemyCount;
	extern EnemyClassConfig enemyClassConfigs[];

	extern GUIState guiState;
	extern GameTime gameTime;
	extern Tower towers[TOWER_MAX_COUNT];
	extern int towerCount;


}