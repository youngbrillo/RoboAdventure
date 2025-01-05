#include "tower.h"

namespace glib::towerdefense
{

    Tower towers[TOWER_MAX_COUNT];
    int towerCount = 0;

    Model towerModels[TOWER_TYPE_COUNT];
    Texture2D palette, spriteSheet;

    // a unit that uses sprites to be drawn
    typedef struct SpriteUnit
    {
        Rectangle srcRect;
        Vector2 offset;
    } SpriteUnit;

    // definition of our archer unit
    SpriteUnit archerUnit = {
        .srcRect = {0, 0, 16, 16},
        .offset = {7, 1},
    };

    void DrawSpriteUnit(SpriteUnit unit, Vector3 position)
    {
        Camera3D camera = currentLevel->camera;
        float size = 0.5f;
        Vector2 offset = Vector2{ unit.offset.x / 16.0f * size, unit.offset.y / 16.0f * size };
        Vector2 scale = Vector2{ unit.srcRect.width / 16.0f * size, unit.srcRect.height / 16.0f * size };
        // we want the sprite to face the camera, so we need to calculate the up vector
        Vector3 forward = Vector3Subtract(camera.target, camera.position);
        Vector3 up = { 0, 1, 0 };
        Vector3 right = Vector3CrossProduct(forward, up);
        up = Vector3Normalize(Vector3CrossProduct(right, forward));
        DrawBillboardPro(camera, spriteSheet, unit.srcRect, position, up, scale, offset, 0, WHITE);
    }

    void TowerInit()
    {
        for (int i = 0; i < TOWER_MAX_COUNT; i++)
        {
            towers[i] = Tower{ 0 };
        }
        towerCount = 0;

        // load a sprite sheet that contains all units
        spriteSheet = LoadTexture("C:/GameDev/tutorials/quakatoo/tower defense/td-tut-46/data//spritesheet.png");

        // we'll use a palette texture to colorize the all buildings and environment art
        palette = LoadTexture("C:/GameDev/tutorials/quakatoo/tower defense/td-tut-46/data/palette.png");
        // The texture uses gradients on very small space, so we'll enable bilinear filtering
        SetTextureFilter(palette, TEXTURE_FILTER_BILINEAR);

        towerModels[TOWER_TYPE_WALL] = LoadModel("C:/GameDev/tutorials/quakatoo/tower defense/td-tut-46/data//wall-0000.glb");

        for (int i = 0; i < TOWER_TYPE_COUNT; i++)
        {
            if (towerModels[i].materials)
            {
                // assign the palette texture to the material of the model (0 is not used afaik)
                towerModels[i].materials[1].maps[MATERIAL_MAP_DIFFUSE].texture = palette;
            }
        }
    }

    static void TowerGunUpdate(Tower* tower)
    {
        if (tower->cooldown <= 0)
        {
            Enemy* enemy = EnemyGetClosestToCastle(tower->x, tower->y, 3.0f);
            if (enemy)
            {
                tower->cooldown = 0.5f;
                // shoot the enemy; determine future position of the enemy
                float bulletSpeed = 1.0f;
                float bulletDamage = 3.0f;
                Vector2 velocity = enemy->simVelocity;
                Vector2 futurePosition = EnemyGetPosition(enemy, gameTime.time - enemy->startMovingTime, &velocity, 0);
                Vector2 towerPosition = { tower->x, tower->y };
                float eta = Vector2Distance(towerPosition, futurePosition) / bulletSpeed;
                for (int i = 0; i < 8; i++) {
                    velocity = enemy->simVelocity;
                    futurePosition = EnemyGetPosition(enemy, gameTime.time - enemy->startMovingTime + eta, &velocity, 0);
                    float distance = Vector2Distance(towerPosition, futurePosition);
                    float eta2 = distance / bulletSpeed;
                    if (fabs(eta - eta2) < 0.01f) {
                        break;
                    }
                    eta = (eta2 + eta) * 0.5f;
                }
                ProjectileTryAdd(PROJECTILE_TYPE_BULLET, enemy, towerPosition, futurePosition,
                    bulletSpeed, bulletDamage);
                enemy->futureDamage += bulletDamage;
            }
        }
        else
        {
            tower->cooldown -= gameTime.deltaTime;
        }
    }

    Tower* TowerGetAt(int16_t x, int16_t y)
    {
        for (int i = 0; i < towerCount; i++)
        {
            if (towers[i].x == x && towers[i].y == y && towers[i].towerType != TOWER_TYPE_NONE)
            {
                return &towers[i];
            }
        }
        return 0;
    }

    Tower* TowerTryAdd(uint8_t towerType, int16_t x, int16_t y)
    {
        if (towerCount >= TOWER_MAX_COUNT)
        {
            return 0;
        }

        Tower* tower = TowerGetAt(x, y);
        if (tower)
        {
            return 0;
        }

        tower = &towers[towerCount++];
        tower->x = x;
        tower->y = y;
        tower->towerType = towerType;
        tower->cooldown = 0.0f;
        tower->damage = 0.0f;
        return tower;
    }

    Tower* GetTowerByType(uint8_t towerType)
    {
        for (int i = 0; i < towerCount; i++)
        {
            if (towers[i].towerType == towerType)
            {
                return &towers[i];
            }
        }
        return 0;
    }

    int GetTowerCosts(uint8_t towerType)
    {
        switch (towerType)
        {
        case TOWER_TYPE_BASE:
            return 0;
        case TOWER_TYPE_GUN:
            return 6;
        case TOWER_TYPE_WALL:
            return 2;
        }
        return 0;
    }

    float TowerGetMaxHealth(Tower* tower)
    {
        switch (tower->towerType)
        {
        case TOWER_TYPE_BASE:
            return 10.0f;
        case TOWER_TYPE_GUN:
            return 3.0f;
        case TOWER_TYPE_WALL:
            return 5.0f;
        }
        return 0.0f;
    }

    void TowerDraw()
    {
        for (int i = 0; i < towerCount; i++)
        {
            Tower tower = towers[i];
            if (tower.towerType == TOWER_TYPE_NONE)
            {
                continue;
            }

            switch (tower.towerType)
            {
            case TOWER_TYPE_BASE:
                DrawCube(Vector3{ (float)tower.x, 0.4f,  (float)tower.y }, 0.8f, 0.8f, 0.8f, MAROON);
                break;
            case TOWER_TYPE_GUN:
                DrawModel(towerModels[TOWER_TYPE_WALL], Vector3{ (float)tower.x, 0.0f,  (float)tower.y }, 1.0f, WHITE);
                DrawSpriteUnit(archerUnit, Vector3{ (float)tower.x, 1.0f,  (float)tower.y });
                break;
            default:
                if (towerModels[tower.towerType].materials)
                {
                    DrawModel(towerModels[tower.towerType], Vector3{ (float)tower.x, 0.0f,  (float)tower.y }, 1.0f, WHITE);
                }
                else {
                    DrawCube(Vector3{ (float)tower.x, 0.5f,  (float)tower.y }, 1.0f, 1.0f, 1.0f, LIGHTGRAY);
                }
                break;
            }
        }
    }

    void TowerUpdate()
    {
        for (int i = 0; i < towerCount; i++)
        {
            Tower* tower = &towers[i];
            switch (tower->towerType)
            {
            case TOWER_TYPE_GUN:
                TowerGunUpdate(tower);
                break;
            }
        }
    }

    void TowerDrawHealthBars(Camera3D camera)
    {
        for (int i = 0; i < towerCount; i++)
        {
            Tower* tower = &towers[i];
            if (tower->towerType == TOWER_TYPE_NONE || tower->damage <= 0.0f)
            {
                continue;
            }

            Vector3 position = Vector3{ (float)tower->x, 0.5f,  (float)tower->y };
            float maxHealth = TowerGetMaxHealth(tower);
            float health = maxHealth - tower->damage;
            float healthRatio = health / maxHealth;

            DrawHealthBar(camera, position, healthRatio, GREEN);
        }
    }

}