#pragma once
#include "GameState.h"
#include "GameManager.h"
#include "Constants.h"
#include "raylib.h"

class MenuState : public GameState
{
public:
    MenuState() : m_titleAlpha(0.0f) {}

    void Enter() override
    {
        GameManager::Instance().Reset();
        m_titleAlpha = 0.0f;
    }

    void Exit() override {}

    const char* Update(float dt) override
    {
        m_titleAlpha += dt * 1.8f;
        if (m_titleAlpha > 1.0f) m_titleAlpha = 1.0f;

        if (IsKeyPressed(KEY_SPACE)) return "Playing";
        if (IsKeyPressed(KEY_ESCAPE)) return "Exit";
        return "";
    }

    void Draw() const override
    {
        ClearBackground({ 5, 5, 15, 255 });  // deep space

        // Stars (static pattern --- deterministic using position)
        for (int i = 0; i < 120; ++i)
        {
            int sx = (i * 173 + 37) % SCREEN_WIDTH;
            int sy = (i * 251 + 113) % SCREEN_HEIGHT;
            DrawPixel(sx, sy, { 200, 200, 220, 200 });
        }

        // Title
        const char* line1 = "STAR WARS";
        const char* line2 = "HORDE";
        Color yellow = Fade({ 255, 220, 0, 255 }, m_titleAlpha);
        Color white = Fade(WHITE, m_titleAlpha);

        int w1 = MeasureText(line1, 70);
        int w2 = MeasureText(line2, 50);
        DrawText(line1, SCREEN_WIDTH / 2 - w1 / 2, 200, 70, yellow);
        DrawText(line2, SCREEN_WIDTH / 2 - w2 / 2, 280, 50, white);

        // Sub
        DrawText("A survivor horde game",
            SCREEN_WIDTH / 2 - MeasureText("A survivor horde game", 20) / 2,
            350, 20, GRAY);

        // Instructions
        Color prompt = Fade(LIGHTGRAY, m_titleAlpha);
        DrawText("SPACE  ---  Play",
            SCREEN_WIDTH / 2 - MeasureText("SPACE  -  Play", 22) / 2,
            440, 22, prompt);
        DrawText("ESC  ---  Quit",
            SCREEN_WIDTH / 2 - MeasureText("ESC  -  Quit", 20) / 2,
            470, 20, Fade(GRAY, m_titleAlpha));

        // Controls reminder
        DrawText("WASD move  |  TAB/Q swap weapon  |  Auto-fire",
            SCREEN_WIDTH / 2 - MeasureText("WASD move  |  TAB/Q swap weapon  |  Auto-fire", 18) / 2,
            640, 18, { 100,100,120,255 });
    }

    const char* GetName() const override { return "Menu"; }

private:
    float m_titleAlpha;
};