#pragma once
#include "GameState.h"
#include "GameManager.h"
#include "Constants.h"
#include "raylib.h"

class GameOverState : public GameState
{
public:
    void Enter() override
    {
        m_finalScore = GameManager::Instance().GetScore();
        m_finalKills = GameManager::Instance().GetKills();
        m_finalWave = GameManager::Instance().GetWave();
        m_timer = 0.0f;
    }

    void Exit() override {}

    const char* Update(float dt) override
    {
        m_timer += dt;
        if (m_timer > 1.2f)
        {
            if (IsKeyPressed(KEY_R) || IsKeyPressed(KEY_SPACE))
                return "Menu";
        }
        return "";
    }

    void Draw() const override
    {
        ClearBackground({ 20, 0, 0, 255 });

        // Background stars
        for (int i = 0; i < 120; ++i)
        {
            int sx = (i * 173 + 37) % SCREEN_WIDTH;
            int sy = (i * 251 + 113) % SCREEN_HEIGHT;
            DrawPixel(sx, sy, { 200, 200, 220, 200 });
        }

        const char* title = "THE DARK SIDE WINS";
        DrawText(title,
            SCREEN_WIDTH / 2 - MeasureText(title, 52) / 2,
            150, 52, { 220, 50, 50, 255 });

        DrawText(TextFormat("Score:  %d", m_finalScore),
            SCREEN_WIDTH / 2 - 120, 260, 30, { 255, 220, 0, 255 });
        DrawText(TextFormat("Kills:  %d", m_finalKills),
            SCREEN_WIDTH / 2 - 120, 300, 30, WHITE);
        DrawText(TextFormat("Wave:   %d", m_finalWave),
            SCREEN_WIDTH / 2 - 120, 340, 30, LIGHTGRAY);

        if (m_timer > 1.2f)
        {
            float a = (m_timer - 1.2f) * 1.5f;
            if (a > 1.0f) a = 1.0f;
            DrawText("SPACE / R  ---  Return to Menu",
                SCREEN_WIDTH / 2 - MeasureText("SPACE / R  -  Return to Menu", 24) / 2,
                450, 24, Fade(WHITE, a));
        }
    }

    const char* GetName() const override { return "GameOver"; }

private:
    int   m_finalScore{ 0 };
    int   m_finalKills{ 0 };
    int   m_finalWave{ 0 };
    float m_timer{ 0.0f };
};