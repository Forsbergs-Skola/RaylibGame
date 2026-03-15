#pragma once
#include "raylib.h"
#include "raymath.h"
#include "GameManager.h"

enum class BulletType { Blaster, Cannon };

class Bullet
{
public:
    Bullet()
        : m_active(false)
        , m_pos({ 0,0 })
        , m_vel({ 0,0 })
        , m_radius(5.0f)
        , m_damage(20.0f)
        , m_lifetime(0.0f)
        , m_type(BulletType::Blaster)
    {
    }

     void Activate(Vector2 pos, Vector2 vel,
        float radius, float damage, float lifetime, BulletType type)
    {
        m_pos = pos;
        m_vel = vel;
        m_radius = radius;
        m_damage = damage;
        m_lifetime = lifetime;
        m_type = type;
        m_active = true;
    }

    void Deactivate() { m_active = false; }
    bool IsActive()   const { return m_active; }

    void Update(float dt)
    {
        if (!m_active) return;
        m_pos.x += m_vel.x * dt;
        m_pos.y += m_vel.y * dt;
        m_lifetime -= dt;
        if (m_lifetime <= 0.0f) Deactivate();
    }

    void Draw() const
    {
        if (!m_active) return;
         Color col = (m_type == BulletType::Blaster)
            ? Color{ 80, 255, 80, 255 }    
        : Color{ 255, 80, 80, 255 };   
        DrawCircleV(m_pos, m_radius, col);
        DrawCircleV(m_pos, m_radius * 0.5f, WHITE);
    }

    Vector2 GetPos()    const { return m_pos; }
    float   GetRadius() const { return m_radius; }
    float   GetDamage() const { return m_damage; }

private:
    bool       m_active;
    Vector2    m_pos;
    Vector2    m_vel;
    float      m_radius;
    float      m_damage;
    float      m_lifetime;
    BulletType m_type;
};