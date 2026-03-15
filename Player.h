#pragma once
#include "raylib.h"
#include "raymath.h"
#include "Constants.h"
#include "GameManager.h"
#include "Weapon.h"
#include "BulletPool.h"
#include "EnemyPool.h"
#include <memory>
#include <vector>
#include <string>

class Player
{
public:
    Player()
    {
        Reset();
        m_weapons.push_back(std::make_unique<BlasterWeapon>());
        m_weapons.push_back(std::make_unique<RapidCannonWeapon>());
        m_weapons.push_back(std::make_unique<LightsaberWeapon>());
        m_weaponIdx = 0;
    }

    void Reset()
    {
        m_pos = { SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f };
        m_moveDir = { 1.0f, 0.0f };
        m_isMoving = false;
        m_iFrames = 0.0f;
    }

    void Update(float dt, BulletPool& bullets, EnemyPool& enemies)
    {
        // WASD Movement
        Vector2 move = { 0, 0 };
        if (IsKeyDown(KEY_W)) move.y -= 1.0f;
        if (IsKeyDown(KEY_S)) move.y += 1.0f;
        if (IsKeyDown(KEY_A)) move.x -= 1.0f;
        if (IsKeyDown(KEY_D)) move.x += 1.0f;

        float len = Vector2Length(move);
        if (len > 0.0f)
        {
            move = Vector2Scale(move, 1.0f / len);
            m_moveDir = move;
            m_isMoving = true;
        }
        else { m_isMoving = false; }

        m_pos.x += move.x * PLAYER_SPEED * dt;
        m_pos.y += move.y * PLAYER_SPEED * dt;

        // Weapon swap: TAB or Q
        if (IsKeyPressed(KEY_TAB) || IsKeyPressed(KEY_Q))
            m_weaponIdx = (m_weaponIdx + 1) % (int)m_weapons.size();

        // Aim direction: Blaster = mouse, others = movement direction
        Vector2 aimDir;
        if (m_weaponIdx == 0)
        {
            Vector2 mouse = GetMousePosition();
            Vector2 toMouse = Vector2Subtract(mouse, m_pos);
            float   d = Vector2Length(toMouse);
            aimDir = (d > 1.0f) ? Vector2Scale(toMouse, 1.0f / d)
                : Vector2{ 1.0f, 0.0f };
        }
        else
        {
            aimDir = m_isMoving ? m_moveDir : Vector2{ 1.0f, 0.0f };
        }

        // Fire / spin saber
        m_weapons[m_weaponIdx]->Fire(m_pos, aimDir, dt, bullets, enemies);

        // i-frames countdown
        if (m_iFrames > 0.0f) m_iFrames -= dt;
    }

    void TakeDamage(float dmg)
    {
        if (m_iFrames > 0.0f) return;
        m_iFrames = PLAYER_IFRAMES;
        GameManager::Instance().DamagePlayer(dmg);
    }

    void Draw() const
    {
        bool  flashing = (m_iFrames > 0.0f);
        Color weapCol = m_weapons[m_weaponIdx]->GetColor();

        // Draw lightsaber blade BEFORE the ship (so ship appears on top)
        m_weapons[m_weaponIdx]->DrawWeapon(m_pos);

        // ── Millennium Falcon drawn with primitives ────────────────────
        // The ship faces the mouse cursor direction
        Vector2 mouse = GetMousePosition();
        Vector2 toMouse = Vector2Subtract(mouse, m_pos);
        float   facing = atan2f(toMouse.y, toMouse.x);

        // Helper lambda to rotate+translate a point around m_pos
        // We use a manual rotation since we can't use lambdas easily in all compilers
        // Instead we precompute sin/cos
        float cosF = cosf(facing);
        float sinF = sinf(facing);

        // Rotate local point (lx,ly) around origin and offset to world pos
#define RX(lx,ly) (m_pos.x + (lx)*cosF - (ly)*sinF)
#define RY(lx,ly) (m_pos.y + (lx)*sinF + (ly)*cosF)

        Color hullCol = flashing ? Color{ 255,255,255,160 } : Color{ 160,160,150,255 };
        Color darkCol = flashing ? Color{ 255,255,255,100 } : Color{ 80, 80, 75, 255 };
        Color cockCol = flashing ? Color{ 255,255,255,200 } : Color{ 100,200,255,255 };
        Color thruster = Color{ 80, 160, 255, 200 };

        // Main saucer hull (oval-ish using a circle + offset circles)
        DrawCircle((int)RX(0, 0), (int)RY(0, 0), 18, hullCol);
        DrawCircle((int)RX(4, 0), (int)RY(4, 0), 14, hullCol);
        DrawCircle((int)RX(-4, 0), (int)RY(-4, 0), 14, hullCol);
        DrawCircle((int)RX(0, 6), (int)RY(0, 6), 12, hullCol);
        DrawCircle((int)RX(0, -6), (int)RY(0, -6), 12, hullCol);

        // Cockpit arm (extending to the right of facing direction)
        DrawCircle((int)RX(8, -10), (int)RY(8, -10), 5, hullCol);
        DrawCircle((int)RX(12, -12), (int)RY(12, -12), 4, hullCol);
        // Cockpit window
        DrawCircle((int)RX(13, -13), (int)RY(13, -13), 3, cockCol);

        // Engine glow at the back
        DrawCircle((int)RX(-16, 4), (int)RY(-16, 4), 4, darkCol);
        DrawCircle((int)RX(-16, -4), (int)RY(-16, -4), 4, darkCol);
        DrawCircle((int)RX(-17, 4), (int)RY(-17, 4), 3, thruster);
        DrawCircle((int)RX(-17, -4), (int)RY(-17, -4), 3, thruster);

        // Weapon ring indicator
        DrawCircleLines((int)m_pos.x, (int)m_pos.y, 22, weapCol);

#undef RX
#undef RY

        // Aim line for ranged weapons
        if (!m_weapons[m_weaponIdx]->IsMelee())
        {
            float d = Vector2Length(toMouse);
            if (d > 0.0f)
            {
                Vector2 dir = Vector2Scale(toMouse, 1.0f / d);
                DrawLineEx(m_pos,
                    { m_pos.x + dir.x * 30.0f, m_pos.y + dir.y * 30.0f },
                    2.0f, weapCol);
            }
        }
    }

    Vector2     GetPos()         const { return m_pos; }
    std::string GetWeaponName()  const { return m_weapons[m_weaponIdx]->GetName(); }
    Color       GetWeaponColor() const { return m_weapons[m_weaponIdx]->GetColor(); }

private:
    Vector2  m_pos;
    Vector2  m_moveDir;
    bool     m_isMoving;
    float    m_iFrames;

    std::vector<std::unique_ptr<IWeapon>> m_weapons;
    int m_weaponIdx;
};