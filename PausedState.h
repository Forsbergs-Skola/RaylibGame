#pragma once
#include "GameState.h"
#include "GameManager.h"
#include "Constants.h"
#include "raylib.h"

class PausedState : public GameState
{
public:
    // PlayingState passes itself so we can draw it underneath
    void SetUnderlay(const GameState* under) { m_underlay = under; }

    void Enter() override { GameManager::Instance().SetPaused(true); }
    void Exit()  override { GameManager::Instance().SetPaused(false); }

    const char* Update(float dt) override
    {
        if (IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_P)) return "Playing";
        if (IsKeyPressed(KEY_Q))                              return "Menu";
        return "";
    }

    void Draw() const override
    {
        // Draw frozen gameplay underneath
        if (m_underlay) m_underlay->Draw();

        // Dark overlay
        DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, { 0,0,0,160 });

        // Pause text
        const char* title = "PAUSED";
        int tw = MeasureText(title, 60);
        DrawText(title, SCREEN_WIDTH / 2 - tw / 2, SCREEN_HEIGHT / 2 - 80, 60, WHITE);

        DrawText("P / ESC  ---  Resume",
            SCREEN_WIDTH / 2 - MeasureText("P / ESC  -  Resume", 22) / 2,
            SCREEN_HEIGHT / 2 + 10, 22, LIGHTGRAY);
        DrawText("Q  ---  Quit to Menu",
            SCREEN_WIDTH / 2 - MeasureText("Q  -  Quit to Menu", 22) / 2,
            SCREEN_HEIGHT / 2 + 40, 22, LIGHTGRAY);
    }

    const char* GetName() const override { return "Paused"; }

private:
    const GameState* m_underlay{ nullptr };
};