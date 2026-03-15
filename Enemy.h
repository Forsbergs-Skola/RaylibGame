#pragma once
#include "raylib.h"
#include "raymath.h"
#include "EnemyType.h"
#include "Constants.h"
#include <cmath>

// ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//  Enemy --- works with EnemyPool (Object Pool) and EnemyFactory
// ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class Enemy
{
public:
    Enemy()
        : m_active(false)
        , m_pos({ 0,0 })
        , m_speed(TROOPER_SPEED)
        , m_health(TROOPER_HEALTH)
        , m_maxHealth(TROOPER_HEALTH)
        , m_damage(TROOPER_DAMAGE)
        , m_radius(TROOPER_RADIUS)
        , m_pointValue(10)
        , m_type(EnemyType::Stormtrooper)
        , m_flashTimer(0.0f)
    {
    }

    // Called by EnemyFactory via EnemyPool::Spawn()
    void SetStats(float speed, float health, float damage,
        float radius, int points, EnemyType type)
    {
        m_speed = speed;
        m_health = health;
        m_maxHealth = health;
        m_damage = damage;
        m_radius = radius;
        m_pointValue = points;
        m_type = type;
    }

    void Activate(Vector2 pos)
    {
        m_pos = pos;
        m_health = m_maxHealth;
        m_active = true;
        m_flashTimer = 0.0f;
    }

    void Deactivate() { m_active = false; }
    bool IsActive()   const { return m_active; }

    void Update(float dt, Vector2 playerPos)
    {
        if (!m_active) return;
        if (m_flashTimer > 0.0f) m_flashTimer -= dt;

        // Walk toward player
        Vector2 dir = Vector2Subtract(playerPos, m_pos);
        float   len = Vector2Length(dir);
        if (len > 0.5f)
        {
            dir = Vector2Scale(dir, 1.0f / len);
            m_pos.x += dir.x * m_speed * dt;
            m_pos.y += dir.y * m_speed * dt;
        }
    }

    void TakeDamage(float dmg)
    {
        m_health -= dmg;
        m_flashTimer = 0.12f;
        if (m_health <= 0.0f) Deactivate();
    }

    void Draw() const
    {
        if (!m_active) return;

        bool flash = (m_flashTimer > 0.0f);
        Color base = (m_type == EnemyType::Stormtrooper)
            ? Color{ 220, 220, 220, 255 }   // white armour
        : Color{ 30, 0, 0, 255 };       // sith dark red/black body
        Color col = flash ? WHITE : base;

        DrawCircleV(m_pos, m_radius, col);

        // Red outline ring for Sith to make them visible
        if (m_type == EnemyType::SithWarrior)
            DrawCircleLines((int)m_pos.x, (int)m_pos.y, m_radius, { 200, 0, 0, 255 });

        // Helmet visor stripe
        Color visor = (m_type == EnemyType::Stormtrooper)
            ? Color{ 50, 50, 50, 255 }
        : Color{ 255, 0, 0, 255 };
        DrawRectangle((int)(m_pos.x - m_radius * 0.45f),
            (int)(m_pos.y - m_radius * 0.25f),
            (int)(m_radius * 0.9f), (int)(m_radius * 0.25f), visor);

        // Health bar
        float bw = m_radius * 2.2f;
        float bh = 5.0f;
        float bx = m_pos.x - bw * 0.5f;
        float by = m_pos.y - m_radius - 10.0f;
        float pct = m_health / m_maxHealth;
        DrawRectangle((int)bx, (int)by, (int)bw, (int)bh, { 60,20,20,255 });
        DrawRectangle((int)bx, (int)by, (int)(bw * pct), (int)bh, { 220,50,50,255 });
    }

    Vector2 GetPos()     const { return m_pos; }
    float   GetRadius()  const { return m_radius; }
    float   GetDamage()  const { return m_damage; }
    int     GetPoints()  const { return m_pointValue; }
    EnemyType GetType()  const { return m_type; }

private:
    bool      m_active;
    Vector2   m_pos;
    float     m_speed;
    float     m_health;
    float     m_maxHealth;
    float     m_damage;
    float     m_radius;
    int       m_pointValue;
    EnemyType m_type;
    float     m_flashTimer;
};