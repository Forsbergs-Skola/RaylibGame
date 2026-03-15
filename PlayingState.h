#pragma once
#include "GameState.h"
#include "GameManager.h"
#include "Player.h"
#include "BulletPool.h"
#include "EnemyPool.h"
#include "EnemySpawner.h"
#include "Enemy.h"
#include "Bullet.h"
#include "Constants.h"
#include "raylib.h"
#include <string>
#include <cmath>

class PlayingState : public GameState
{
public:
    PlayingState()
        : m_bulletPool(BULLET_POOL_SIZE)
        , m_enemyPool(ENEMY_POOL_SIZE)
        , m_gameTime(0.0f)
    {
    }

    void Enter() override
    {
        // Reset everything --- pools deactivate, player re-centres
        m_bulletPool.DeactivateAll();
        m_enemyPool.DeactivateAll();
        m_player.Reset();
        m_gameTime = 0.0f;
    }

    void Exit() override
    {
        m_bulletPool.DeactivateAll();
        m_enemyPool.DeactivateAll();
    }

    const char* Update(float dt) override
    {
        if (IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_P))
            return "Paused";

        if (GameManager::Instance().IsGameOver())
            return "GameOver";

        m_gameTime += dt;

        // Update player (fires into bullet pool)
        m_player.Update(dt, m_bulletPool, m_enemyPool);

        // Spawn enemies
        m_spawner.Update(dt, m_gameTime, m_player.GetPos(), m_enemyPool);

        // Update enemies --- walk toward player
        m_enemyPool.UpdateAll(dt, m_player.GetPos());

        // Update bullets
        m_bulletPool.UpdateAll(dt);

        // ------ Bullet vs Enemy collision ---------------------------------------------------------------------------------------------------------
        for (int b = 0; b < m_bulletPool.GetPoolSize(); ++b)
        {
            Bullet& bullet = m_bulletPool[b];
            if (!bullet.IsActive()) continue;

            for (int e = 0; e < m_enemyPool.GetPoolSize(); ++e)
            {
                Enemy& enemy = m_enemyPool[e];
                if (!enemy.IsActive()) continue;

                if (CheckCollisionCircles(bullet.GetPos(), bullet.GetRadius(),
                    enemy.GetPos(), enemy.GetRadius()))
                {
                    enemy.TakeDamage(bullet.GetDamage());
                    bullet.Deactivate();

                    if (!enemy.IsActive())
                    {
                        GameManager::Instance().AddScore(enemy.GetPoints());
                        GameManager::Instance().AddKill();
                    }
                    break;
                }
            }
        }

        // ------ Enemy vs Player collision ---------------------------------------------------------------------------------------------------------
        for (int e = 0; e < m_enemyPool.GetPoolSize(); ++e)
        {
            Enemy& enemy = m_enemyPool[e];
            if (!enemy.IsActive()) continue;

            if (CheckCollisionCircles(m_player.GetPos(), PLAYER_RADIUS,
                enemy.GetPos(), enemy.GetRadius()))
            {
                m_player.TakeDamage(enemy.GetDamage());
            }
        }

        return "";
    }

    void Draw() const override
    {
        // ------ Background: space + grid ------------------------------------------------------------------------------------------------------
        ClearBackground({ 5, 8, 5, 255 });

        // Stars
        for (int i = 0; i < 120; ++i)
        {
            int sx = (i * 173 + 37) % SCREEN_WIDTH;
            int sy = (i * 251 + 113) % SCREEN_HEIGHT;
            DrawPixel(sx, sy, { 180, 180, 210, 200 });
        }

        // Dim grid
        Color grid = { 18, 30, 18, 255 };
        for (int x = 0; x < SCREEN_WIDTH; x += 80) DrawLine(x, 0, x, SCREEN_HEIGHT, grid);
        for (int y = 0; y < SCREEN_HEIGHT; y += 80) DrawLine(0, y, SCREEN_WIDTH, y, grid);

        // Game objects
        m_enemyPool.DrawAll();
        m_bulletPool.DrawAll();
        m_player.Draw();

        // ------ HUD ------------------------------------------------------------------------------------------------------------------------------------------------------------------------
        DrawHUD();
    }

    const char* GetName() const override { return "Playing"; }

    // Expose for PausedState to draw underneath
    float GetGameTime() const { return m_gameTime; }

private:
    Player       m_player;
    BulletPool   m_bulletPool;
    EnemyPool    m_enemyPool;
    EnemySpawner m_spawner;
    float        m_gameTime;

    void DrawHUD() const
    {
        GameManager& gm = GameManager::Instance();

        // ------ Health bar ------------------------------------------------------------------------------------------------------------------------------------------
        float pct = gm.GetPlayerHealth() / PLAYER_MAX_HEALTH;
        if (pct < 0.0f) pct = 0.0f;
        DrawRectangle(20, 20, 200, 22, { 60,20,20,255 });
        DrawRectangle(20, 20, (int)(200 * pct), 22, { 220,50,50,255 });
        DrawRectangleLines(20, 20, 200, 22, WHITE);
        DrawText(TextFormat("HP: %d", (int)gm.GetPlayerHealth()),
            26, 23, 16, WHITE);

        // ------ Timer ---------------------------------------------------------------------------------------------------------------------------------------------------------
        int mins = (int)m_gameTime / 60;
        int secs = (int)m_gameTime % 60;
        char tbuf[16];
        snprintf(tbuf, sizeof(tbuf), "%02d:%02d", mins, secs);
        int tw = MeasureText(tbuf, 30);
        DrawText(tbuf, SCREEN_WIDTH / 2 - tw / 2, 16, 30, WHITE);

        // ------ Wave ------------------------------------------------------------------------------------------------------------------------------------------------------------
        std::string waveStr = "Wave " + std::to_string(gm.GetWave());
        int ww = MeasureText(waveStr.c_str(), 20);
        DrawText(waveStr.c_str(), SCREEN_WIDTH / 2 - ww / 2, 52, 20, { 200,200,0,255 });

        // ------ Score / Kills ------------------------------------------------------------------------------------------------------------------------------------
        DrawText(TextFormat("Score: %d", gm.GetScore()), 20, 52, 20, { 200,200,0,255 });
        DrawText(TextFormat("Kills: %d", gm.GetKills()), 20, 76, 18, GRAY);

        // ------ Weapon ------------------------------------------------------------------------------------------------------------------------------------------------------
        Color wc = m_player.GetWeaponColor();
        std::string ws = "Weapon: " + m_player.GetWeaponName() + "  [TAB/Q]";
        DrawText(ws.c_str(), 20, SCREEN_HEIGHT - 34, 18, wc);

        // ------ Pool debug (small, bottom-right) ---------------------------------------------------------------------------
        DrawText(TextFormat("Bullets: %d  Enemies: %d",
            m_bulletPool.GetActiveCount(),
            m_enemyPool.GetActiveCount()),
            SCREEN_WIDTH - 240, SCREEN_HEIGHT - 28, 16, { 80,80,80,255 });
    }
};