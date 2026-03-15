#pragma once
#include "EnemyPool.h"
#include "EnemyFactory.h"
#include "GameManager.h"
#include "Constants.h"
#include "raylib.h"
#include <cmath>

// Spawns enemies in rings around the player.
// Interval shrinks and group size grows over time (VG requirement).
class EnemySpawner
{
public:
    void Update(float dt, float gameTime, Vector2 playerPos, EnemyPool& pool)
    {
        m_timer += dt;

        // ------ Difficulty ramp ------------------------------------------------------------------------------------------------------------------------------------------
        float diffMult = 1.0f + gameTime / 60.0f;
        float interval = SPAWN_INTERVAL_START / diffMult;
        if (interval < SPAWN_INTERVAL_MIN) interval = SPAWN_INTERVAL_MIN;

        int count = SPAWN_COUNT_START + (int)(gameTime / 18.0f);
        if (count > SPAWN_COUNT_MAX) count = SPAWN_COUNT_MAX;

        if (m_timer < interval) return;
        m_timer = 0.0f;

        // Advance wave counter in GameManager every 5 spawns
        m_spawnCount += count;
        if (m_spawnCount >= 5 * GameManager::Instance().GetWave())
            GameManager::Instance().NextWave();

        int wave = GameManager::Instance().GetWave();

        for (int i = 0; i < count; ++i)
        {
            float angle = ((float)GetRandomValue(0, 10000) / 10000.0f)
                * 2.0f * PI;
            float dist = SPAWN_RADIUS
                + (float)GetRandomValue(0, 80);

            Vector2 sp =
            {
                playerPos.x + cosf(angle) * dist,
                playerPos.y + sinf(angle) * dist
            };

            EnemyType type = EnemyFactory::RandomTypeForWave(wave);
            pool.Spawn(type, sp);
        }
    }

private:
    float m_timer{ 0.0f };
    int   m_spawnCount{ 0 };
};