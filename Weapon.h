#pragma once
#include "raylib.h"
#include "raymath.h"
#include "BulletPool.h"
#include "EnemyPool.h"
#include "Enemy.h"
#include "GameManager.h"
#include "Constants.h"
#include <string>
#include <cmath>

// IWeapon --- abstract interface (Strategy Pattern)
class IWeapon
{
public:
    virtual ~IWeapon() = default;

    virtual void Fire(Vector2 origin, Vector2 aimDir,
        float dt, BulletPool& pool, EnemyPool& enemies) = 0;

    virtual void DrawWeapon(Vector2 origin) const {}

    virtual std::string GetName()  const = 0;
    virtual Color       GetColor() const = 0;
    virtual bool        IsMelee()  const { return false; }

protected:
    float m_cooldown{ 0.0f };
};

// ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//  BlasterWeapon --- single accurate bolt, aimed at the mouse cursor
// ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class BlasterWeapon : public IWeapon
{
public:
    void Fire(Vector2 origin, Vector2 aimDir,
        float dt, BulletPool& pool, EnemyPool& enemies) override
    {
        m_cooldown -= dt;
        if (m_cooldown > 0.0f) return;
        m_cooldown = BLASTER_FIRE_RATE;

        Vector2 vel = { aimDir.x * BLASTER_BULLET_SPEED,
                        aimDir.y * BLASTER_BULLET_SPEED };

        pool.Spawn(origin, vel,
            BLASTER_BULLET_RAD, BLASTER_BULLET_DMG,
            BLASTER_BULLET_LIFE, BulletType::Blaster);
    }

    std::string GetName()  const override { return "Blaster (mouse aim)"; }
    Color       GetColor() const override { return { 80,255,80,255 }; }
};

// ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//  RapidCannonWeapon --- spread burst in movement direction
// ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class RapidCannonWeapon : public IWeapon
{
public:
    void Fire(Vector2 origin, Vector2 aimDir,
        float dt, BulletPool& pool, EnemyPool& enemies) override
    {
        m_cooldown -= dt;
        if (m_cooldown > 0.0f) return;
        m_cooldown = CANNON_FIRE_RATE;

        float baseAngle = atan2f(aimDir.y, aimDir.x);
        float spreadRad = CANNON_SPREAD_DEG * DEG2RAD;

        for (int i = 0; i < CANNON_PELLETS; ++i)
        {
            float t = (CANNON_PELLETS == 1) ? 0.0f
                : (float)i / (CANNON_PELLETS - 1) - 0.5f;
            float angle = baseAngle + t * spreadRad;

            Vector2 vel = { cosf(angle) * CANNON_BULLET_SPEED,
                            sinf(angle) * CANNON_BULLET_SPEED };

            pool.Spawn(origin, vel,
                CANNON_BULLET_RAD, CANNON_BULLET_DMG,
                CANNON_BULLET_LIFE, BulletType::Cannon);
        }
    }

    std::string GetName()  const override { return "Rapid Cannon (move dir)"; }
    Color       GetColor() const override { return { 255,80,80,255 }; }
};

// LightsaberWeapon --- spinning blade that orbits the player and damages enemies on contact
class LightsaberWeapon : public IWeapon
{
public:
    LightsaberWeapon() : m_angle(0.0f) {}

    void Fire(Vector2 origin, Vector2 aimDir,
        float dt, BulletPool& pool, EnemyPool& enemies) override
    {
        // Spin the blade continuously
        m_angle += SABER_SPIN_SPEED * dt;

        // Check every frame along the full blade length
        for (int i = 0; i < enemies.GetPoolSize(); ++i)
        {
            Enemy& e = enemies[i];
            if (!e.IsActive()) continue;

            // Sample many points along the blade for reliable detection
            for (float t = 0.0f; t <= 1.0f; t += 0.1f)
            {
                float dist = 18.0f + SABER_BLADE_LEN * t;
                Vector2 bladePoint = {
                    origin.x + cosf(m_angle) * dist,
                    origin.y + sinf(m_angle) * dist
                };
                if (CheckCollisionCircles(bladePoint, 10.0f,
                    e.GetPos(), e.GetRadius()))
                {
                    e.TakeDamage(SABER_DAMAGE);
                    if (!e.IsActive())
                    {
                        GameManager::Instance().AddScore(e.GetPoints());
                        GameManager::Instance().AddKill();
                    }
                    break;
                }
            }
        }
    }

    void DrawWeapon(Vector2 origin) const override
    {
        // Handle start (at player edge)
        Vector2 handleStart = {
            origin.x + cosf(m_angle) * 18.0f,
            origin.y + sinf(m_angle) * 18.0f
        };
        Vector2 handleEnd = {
            origin.x + cosf(m_angle) * 30.0f,
            origin.y + sinf(m_angle) * 30.0f
        };
        // Blade tip
        Vector2 bladeEnd = {
            origin.x + cosf(m_angle) * (30.0f + SABER_BLADE_LEN),
            origin.y + sinf(m_angle) * (30.0f + SABER_BLADE_LEN)
        };

        // Handle (dark grey)
        DrawLineEx(handleStart, handleEnd, 5.0f, { 80, 80, 80, 255 });

        // Outer glow
        DrawLineEx(handleEnd, bladeEnd, 10.0f, { 0, 180, 255, 50 });
        // Mid glow
        DrawLineEx(handleEnd, bladeEnd, 5.0f, { 80, 210, 255, 130 });
        // Core
        DrawLineEx(handleEnd, bladeEnd, 2.0f, { 220, 245, 255, 255 });
    }

    bool        IsMelee()  const override { return true; }
    std::string GetName()  const override { return "Lightsaber (spinning)"; }
    Color       GetColor() const override { return { 0, 200, 255, 255 }; }

private:
    float m_angle;
};