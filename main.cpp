#include "raylib.h"
#include "Constants.h"
#include "StateManager.h"
#include "MenuState.h"
#include "PlayingState.h"
#include "PausedState.h"
#include "GameOverState.h"
#include <string>
#include <cstdlib>
#include <ctime>

int main()
{
    srand((unsigned)time(nullptr));

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Star Wars Horde");
    SetTargetFPS(TARGET_FPS);

    // ------ Create states ------------------------------------------------------------------------------------------------------------------------------------------------------------------
    MenuState* menuState = new MenuState();
    PlayingState* playState = new PlayingState();
    PausedState* pauseState = new PausedState();
    GameOverState* gameOverState = new GameOverState();

    // PausedState draws PlayingState underneath
    pauseState->SetUnderlay(playState);

    // ------ Register with manager ------------------------------------------------------------------------------------------------------------------------------------------
    StateManager stateManager;
    stateManager.AddState("Menu", menuState);
    stateManager.AddState("Playing", playState);
    stateManager.AddState("Paused", pauseState);
    stateManager.AddState("GameOver", gameOverState);

    stateManager.ChangeState("Menu");

    // ------ Game loop --- main.cpp stays CLEAN ------------------------------------------------------------------------------------------------------
    while (!WindowShouldClose())
    {
        // "Exit" state name from MenuState quits the loop
        if (std::string(stateManager.GetCurrentName()) == "Exit") break;

        float dt = GetFrameTime();

        stateManager.Update(dt);

        BeginDrawing();
        stateManager.Draw();
        DrawFPS(SCREEN_WIDTH - 78, 8);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
