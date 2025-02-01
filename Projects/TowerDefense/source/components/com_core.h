#pragma once
#include <gamelib/gameLib.h>

namespace td
{

	struct GameTime
	{
		float time = 0.0f;
		float deltaTime = 0.0f;
	};

	inline GameTime* GetGameTime(entt::registry& registry)
	{
		for (auto&& [e, gt] : registry.view<GameTime>().each())
		{
			return &gt;
		}

		return nullptr;
	}
	struct Timer
	{
		float duration = 1.0f, progress = 0.0f;
		inline bool tick(float dt)
		{
			if (progress >= duration)
			{
				progress = duration;
				return true;
			}
			progress += dt;

			return progress >= duration;
		}

		inline void set(float v)
		{
			progress = 0;
			duration = v;
		}
		inline void reset()
		{
			set(duration);
		}

	};
	struct iVec2 { 
		int x = 0, y = 0; 
		inline bool operator==(const iVec2& other) const
		{
			return x == other.x && y == other.y;
		}
		inline bool operator!=(const iVec2& other) const
		{
			return !(*this == other);
		}
	};

	inline iVec2 operator + (const iVec2& lhs, const iVec2& rhs)
	{
		return iVec2{ lhs.x + rhs.x, lhs.y + rhs.y };
	}

	inline iVec2 operator += (iVec2& lhs, const iVec2& rhs)
	{
		lhs =  iVec2{ lhs.x + rhs.x, lhs.y + rhs.y };
		return lhs;
	}
	inline iVec2 operator - (const iVec2& lhs, const iVec2& rhs)
	{
		return iVec2{ lhs.x - rhs.x, lhs.y - rhs.y };
	}
	inline iVec2 operator -= (iVec2& lhs, const iVec2& rhs)
	{
		lhs = iVec2{ lhs.x - rhs.x, lhs.y - rhs.y };
		return lhs;
	}

	struct EnemyId
	{
		uint16_t index, generation;
	};

	enum TOWER_TYPE
	{
		TOWER_TYPE_NONE = 0,
		TOWER_TYPE_BASE = 1,
		TOWER_TYPE_GUN	= 2
	};
	constexpr int ENTITY_MAX_COUNT = 400;
	struct Tower
	{
		iVec2 pos;
		int type;
		Timer cooldown;
		float range = 3.0f;
	};
	class EnemySystem;
	class ProjectileSystem;

	class TowerSystem
	{
		Tower towers[ENTITY_MAX_COUNT];
		int count = 0;
		entt::registry& ecs;
	public:
		TowerSystem(entt::registry& registry);
		void Init();
		Tower* GetAt(iVec2 position);
		Tower* TryAdd(int type, iVec2 position);

		void Update(EnemySystem& enemies, ProjectileSystem& projectiles, float dt);
		void GunUpdate(EnemySystem& enemies, ProjectileSystem& projectiles, Tower& tower, float dt);
		void Draw();
	};

	enum ENEMY_TYPE
	{
		ENEMY_TYPE_NONE = 0,
		ENEMY_TYPE_MINION = 1,
	};


	struct Enemy
	{
		iVec2 pos;
		iVec2 next;
		uint16_t generation;
		float startMovingTime;
		int type;
	};

	class EnemySystem
	{
		Enemy enemies[ENTITY_MAX_COUNT];
		int count;
		entt::registry& ecs;
		Timer spawnTimer;
	public:
		EnemySystem(entt::registry& registry);

		void Init();
		Enemy* TryAdd(int enemyType, iVec2 position);
		float EnemyGetCurrentSpeed(Enemy* e);
		Enemy* GetClosestToCastle(iVec2 tower, float range);
		EnemyId GetId(Enemy* enemy);
		Enemy* TryResolve(EnemyId enemyId);
		void Update(float dt);
		void Draw();

	};
	enum PROJECTILE_TYPE
	{
		PROJECTILE_TYPE_NONE = 0,
		PROJECTILE_TYPE_BULLET
	};
	struct Projectile
	{
		int type;
		float shootTime;
		float arrivalTime;
		float damage;
		Vector2 position, target, directionNormal;
		EnemyId targetEnemy;
	};

	class ProjectileSystem
	{
		Projectile projectiles[ENTITY_MAX_COUNT];
		int count;
		entt::registry& ecs;
	public:
		ProjectileSystem(entt::registry& registry);
		void Init();
		Projectile* TryAdd(EnemySystem& enemies, int type, Enemy* enemy, Vector2 position, Vector2 target, float speed, float damage);
		void Update(EnemySystem& enemies, float dt);
		void Draw();
	};

}