#include "projectile.h"

namespace glib::towerdefense
{
    static Projectile projectiles[PROJECTILE_MAX_COUNT];
    static int projectileCount = 0;

    void ProjectileInit()
    {
        for (int i = 0; i < PROJECTILE_MAX_COUNT; i++)
        {
            projectiles[i] = Projectile{ 0 };
        }
    }

    void ProjectileDraw()
    {
        for (int i = 0; i < projectileCount; i++)
        {
            Projectile projectile = projectiles[i];
            if (projectile.projectileType == PROJECTILE_TYPE_NONE)
            {
                continue;
            }
            float transition = (gameTime.time - projectile.shootTime) / (projectile.arrivalTime - projectile.shootTime);
            if (transition >= 1.0f)
            {
                continue;
            }
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
                DrawCube(Vector3 { x, 0.2f, y }, size, size, size, RED);
            }
        }
    }

    void ProjectileUpdate()
    {
        for (int i = 0; i < projectileCount; i++)
        {
            Projectile* projectile = &projectiles[i];
            if (projectile->projectileType == PROJECTILE_TYPE_NONE)
            {
                continue;
            }
            float transition = (gameTime.time - projectile->shootTime) / (projectile->arrivalTime - projectile->shootTime);
            if (transition >= 1.0f)
            {
                projectile->projectileType = PROJECTILE_TYPE_NONE;
                Enemy* enemy = EnemyTryResolve(projectile->targetEnemy);
                if (enemy)
                {
                    EnemyAddDamage(enemy, projectile->damage);
                }
                continue;
            }
        }
    }

    Projectile* ProjectileTryAdd(uint8_t projectileType, Enemy* enemy, Vector2 position, Vector2 target, float speed, float damage)
    {
        for (int i = 0; i < PROJECTILE_MAX_COUNT; i++)
        {
            Projectile* projectile = &projectiles[i];
            if (projectile->projectileType == PROJECTILE_TYPE_NONE)
            {
                projectile->projectileType = projectileType;
                projectile->shootTime = gameTime.time;
                projectile->arrivalTime = gameTime.time + Vector2Distance(position, target) / speed;
                projectile->damage = damage;
                projectile->position = position;
                projectile->target = target;
                projectile->directionNormal = Vector2Normalize(Vector2Subtract(target, position));
                projectile->targetEnemy = EnemyGetId(enemy);
                projectileCount = projectileCount <= i ? i + 1 : projectileCount;
                return projectile;
            }
        }
        return 0;
    }
}