#include "com_core.h"

td::TowerSystem::TowerSystem(entt::registry& registry)
	: ecs(registry)
{
	glib::Entity;
}

void td::TowerSystem::Init()
{
	for (int i = 0; i < ENTITY_MAX_COUNT; i++)
	{
		towers[i] = Tower{ 0 };
	}
	count = 0;
}

td::Tower* td::TowerSystem::GetAt(iVec2 position)
{
    for (int i = 0; i < count; i++)
    {
      if (towers[i].pos == position)
      {
        return &towers[i];
      }
    }
	return nullptr;
}

td::Tower* td::TowerSystem::TryAdd(int type, iVec2 position)
{
	if (count >= ENTITY_MAX_COUNT)
		return nullptr;
	Tower* tower = GetAt(position);
	if (tower)
		return nullptr;

	tower = &towers[count++];
	tower->pos = position;
	tower->type = type;
	return tower;
}

void td::TowerSystem::Update(EnemySystem& enemies, ProjectileSystem& projectiles, float dt)
{
	for (int i = 0; i < count; i++)
	{
		Tower& t = towers[i];
		switch (t.type)
		{
		case TOWER_TYPE_GUN:
			GunUpdate(enemies, projectiles, t, dt);
			break;
		}
	}
}

void td::TowerSystem::GunUpdate(EnemySystem& enemies, ProjectileSystem& projectiles, Tower& tower, float dt)
{
	if (tower.cooldown.tick(dt))
	{
		Enemy* enemy = enemies.GetClosestToCastle(tower.pos, tower.type);
		if (enemy)
		{
			tower.cooldown.reset();
			//shoot the enmy
			projectiles.TryAdd(enemies, PROJECTILE_TYPE_BULLET, enemy,
				Vector2{ (float)tower.pos.x, (float)tower.pos.y },
				Vector2{ (float)enemy->pos.x, (float)enemy->pos.y },
				5.0f, 1.0f
				);
		}
	}
}

void td::TowerSystem::Draw()
{
	for (int i = 0; i < count; i++)
	{
		Tower& tower = towers[i];
		DrawCube(Vector3{ (float)tower.pos.x, 0.125f,(float)tower.pos.y }, 1.0f, 0.25f, 1.0f, GRAY);
		switch (tower.type)
		{
		case td::TOWER_TYPE_BASE:
			DrawCube(Vector3{ (float)tower.pos.x, 0.4f,  (float)tower.pos.y }, 0.8f, 0.8f, 0.8f, MAROON);
			break;
		case td::TOWER_TYPE_GUN:
			DrawCube(Vector3 { (float)tower.pos.x, 0.2f,  (float)tower.pos.y }, 0.8f, 0.4f, 0.8f, DARKPURPLE);
			break;
		case td::TOWER_TYPE_NONE:
		default:
			break;
		}
	}

}

td::EnemySystem::EnemySystem(entt::registry& registry)
	: ecs(registry)
{
}

void td::EnemySystem::Init()
{
	for (int i = 0; i < ENTITY_MAX_COUNT; i++)
	{
		enemies[i] = Enemy{ 0 };
	}
	count = 0;
}

td::Enemy* td::EnemySystem::TryAdd(int enemyType, iVec2 position)
{
	Enemy* spawn = nullptr;
    for (int i = 0; i < count; i++)
    {
		Enemy *enemy = &enemies[i];
		if (enemy->type == ENEMY_TYPE_NONE)
		{
			spawn = enemy;
			break;
		}
   }
 
   if (count < ENTITY_MAX_COUNT && spawn == nullptr)
   {
     spawn = &enemies[count++];
   }
 
   if (spawn)
   {
		spawn->pos = position;
		spawn->next = position;
		spawn->type = enemyType;
		spawn->startMovingTime = 0;
		spawn->generation++;
   }
 
   return spawn;
}

float td::EnemySystem::EnemyGetCurrentSpeed(Enemy* e)
{
	switch (e->type)
	{
	case td::ENEMY_TYPE_MINION:
		return 1.0f;
	}

	return 0.33f;
}

td::Enemy* td::EnemySystem::GetClosestToCastle(iVec2 tower, float range)
{
	iVec2 castle = { 0, 0 };
	Enemy* closest = nullptr;
	int16_t closestDistance = 0;
	float range2 = range * range;
	for (int i = 0; i < count; i++)
	{
		Enemy* enemy = &enemies[i];
		if (enemy->type == ENEMY_TYPE_NONE)
		{
		continue;
		}

		iVec2 d = castle - enemy->pos;
		int16_t distance = abs(d.x) + abs(d.y);
		if (!closest || distance < closestDistance)
		{
			iVec2 td = tower - enemy->pos;
			float tdistance2 = td.x * td.x + td.y * td.y;
			if (tdistance2 <= range2)
			{
				closest = enemy;
				closestDistance = distance;
			}
		}
	}
	return closest;
}

td::EnemyId td::EnemySystem::GetId(Enemy* enemy)
{
	return EnemyId
	{
		.index = uint16_t(enemy - enemies),
		.generation = enemy->generation
	};// (enemy - enemies, enemy->generation);
}

td::Enemy* td::EnemySystem::TryResolve(EnemyId enemyId)
{
	if (enemyId.index >= count)
		return nullptr;

	Enemy* enemy = &enemies[enemyId.index];
	if (enemy->generation != enemyId.generation || enemy->type == ENEMY_TYPE_NONE)
		return nullptr;

	return enemy;
}

void td::EnemySystem::Update(float dt)
{
	const iVec2 castle = { 0, 0 };

	if (GameTime* gameTime = GetGameTime(ecs))
	{
		for (int i = 0; i < count; i++)
		{
			Enemy& enemy = enemies[i];
			if (enemy.type == ENEMY_TYPE_NONE)
				continue;

			float speed = 1.0f;
			float transition = (gameTime->time - enemy.startMovingTime) * speed;

			if (transition >= 1.0f)
			{
				enemy.startMovingTime = gameTime->time;
				enemy.pos = enemy.next;

				iVec2 delta = castle - enemy.pos;
				if (delta == iVec2{ 0, 0 })
				{
					enemy.type = ENEMY_TYPE_NONE;
					continue;
				}
				if (abs(delta.x) > abs(delta.y))
				{
					enemy.next = enemy.pos; 
					enemy.next.x += delta.x > 0 ? 1 : -1;
				}
				else
				{
					enemy.next = enemy.pos;
					enemy.next.y += delta.y > 0 ? 1 : -1;
				}
			}
		}
	}

	if (spawnTimer.tick(dt))
	{
		spawnTimer.reset();
		int randValue = GetRandomValue(-5, 5);
		int randside = GetRandomValue(0, 3);

		iVec2 p = { 
			randside == 0 ? -5 : randside == 1 ? 5 : randValue,
			randside == 2 ? -5 : randside == 3 ? 5 : randValue
		};

		TryAdd(ENEMY_TYPE_MINION, p);
	}
}

void td::EnemySystem::Draw()
{
	GameTime* gameTime = GetGameTime(ecs);

	for (int i = 0; i < count; i++)
	{
		Enemy& e = enemies[i];
		float speed = EnemyGetCurrentSpeed(&e);
		float transition = (gameTime->time - e.startMovingTime) * speed;
		float x = e.pos.x + (e.next.x - e.pos.x) * transition;
		float y = e.pos.y + (e.next.y - e.pos.y) * transition;
		switch (e.type)
		{
		case td::ENEMY_TYPE::ENEMY_TYPE_MINION:
		default:
			DrawCube(Vector3{x, 0.2f,y }, 0.4f, 0.4f, 0.4f, GREEN);
			break;
		}
	}
}

td::ProjectileSystem::ProjectileSystem(entt::registry& registry)
	: ecs(registry)
{
}

void td::ProjectileSystem::Init()
{
   for (int i = 0; i < ENTITY_MAX_COUNT; i++)
   {
     projectiles[i] = Projectile{ 0 };
   }
   count = 0;
}

td::Projectile* td::ProjectileSystem::TryAdd(EnemySystem& enemies, int type, Enemy* enemy, Vector2 position, Vector2 target, float speed, float damage)
{
	GameTime* gameTime = GetGameTime(ecs);
	if (gameTime == nullptr)
		return nullptr;
	for (int i = 0; i < ENTITY_MAX_COUNT; i++)
	{
		Projectile *projectile = &projectiles[i];
		if (projectile->type == PROJECTILE_TYPE_NONE)
		{
			projectile->type = type;
			projectile->shootTime = gameTime->time;
			projectile->arrivalTime = gameTime->time + Vector2Distance(position, target) / speed;
			projectile->damage = damage;
			projectile->position = position;
			projectile->target = target;
			projectile->directionNormal = Vector2Normalize(Vector2Subtract(target, position));
			projectile->targetEnemy = enemies.GetId(enemy);
			count = count <= i ? i + 1 : count;
			return projectile;
		}
	}
	return nullptr;
}

void td::ProjectileSystem::Update(EnemySystem& enemies, float dt)
{
	GameTime* gameTime = GetGameTime(ecs);
	if (gameTime == nullptr)
		return;

   for (int i = 0; i < count; i++)
   {
		Projectile *projectile = &projectiles[i];
		if (projectile->type == PROJECTILE_TYPE_NONE)
			continue;
		float transition = (gameTime->time - projectile->shootTime) / (projectile->arrivalTime - projectile->shootTime);
		if (transition >= 1.0f)
		{
			projectile->type = PROJECTILE_TYPE_NONE;
			Enemy *enemy = enemies.TryResolve(projectile->targetEnemy);
			if (enemy)
				enemy->type = ENEMY_TYPE_NONE;
			continue;
		}
   }
}

void td::ProjectileSystem::Draw()
{
	GameTime* gameTime = GetGameTime(ecs);
	if (gameTime == nullptr)
		return;
	for (int i = 0; i < count; i++)
	{
		Projectile& projectile = projectiles[i];
		if (projectile.type == PROJECTILE_TYPE_NONE)
			continue;
		float transition = (gameTime->time - projectile.shootTime) / (projectile.arrivalTime - projectile.shootTime);
		if (transition >= 1.0f)
			continue;
		Vector2 position = Vector2Lerp(projectile.position, projectile.target, transition);
		float x = position.x;
		float y = position.y;
		float dx = projectile.directionNormal.x;
		float dy = projectile.directionNormal.y;
		for (float d = 1.0f; d > 0.0f; d -= 0.25f)
		{
			x -= dx * 0.1f;
			y -= dy * 0.1f;
			float size = 0.1f * d;
			DrawCube(Vector3{x, 0.2f, y}, size, size, size, RED);
		}
	}
}
