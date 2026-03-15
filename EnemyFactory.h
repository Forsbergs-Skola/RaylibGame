#pragma once
#include "Enemy.h"
#include "EnemyType.h"
#include "Constants.h"
#include "raylib.h"

// ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//  Design Pattern 3: FACTORY
//  All enemy configurations live in ONE place.
//  Callers just say which type they want --- no repeated SetStats() scattered
//  around the codebase.
// ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class EnemyFactory
{
public:
    // Configure an existing Enemy (used together with EnemyPool)
    static void Configure(Enemy* enemy, EnemyType type)
    {
        switch (type)
        {
        case EnemyType::Stormtrooper:
            enemy->SetStats(
                TROOPER_SPEED,    // speed
                TROOPER_HEALTH,   // health
                TROOPER_DAMAGE,   // damage
                TROOPER_RADIUS,   // radius
                10,               // score points
                EnemyType::Stormtrooper
            );
            break;

        case EnemyType::SithWarrior:
            enemy->SetStats(
                SITH_SPEED,
                SITH_HEALTH,
                SITH_DAMAGE,
                SITH_RADIUS,
                35,               // worth more points
                EnemyType::SithWarrior
            );
            break;
        }
    }

    // Get a random type weighted by the current wave
    static EnemyType RandomTypeForWave(int wave)
    {
        int r = GetRandomValue(0, 100);

        // Wave 1-2: only Stormtroopers
        if (wave <= 2) return EnemyType::Stormtrooper;

        // Wave 3-4: 20% chance of Sith
        if (wave <= 4) return (r < 80) ? EnemyType::Stormtrooper
            : EnemyType::SithWarrior;

        // Wave 5+: up to 50% Sith
        int sithChance = 20 + (wave - 4) * 6;
        if (sithChance > 50) sithChance = 50;
        return (r < sithChance) ? EnemyType::SithWarrior
            : EnemyType::Stormtrooper;
    }
};