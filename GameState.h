#pragma once
#include "raylib.h"

// ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//  Design Pattern 4: STATE
//  Each game screen is its own class.  The StateManager calls Enter/Exit/
//  Update/Draw on the active state only.
// ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class GameState
{
public:
    virtual ~GameState() = default;

    virtual void        Enter() = 0;
    virtual void        Exit() = 0;
    // Returns name of next state, or "" to stay
    virtual const char* Update(float dt) = 0;
    virtual void        Draw()   const = 0;
    virtual const char* GetName()const = 0;
};