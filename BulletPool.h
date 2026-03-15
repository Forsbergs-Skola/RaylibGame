#pragma once
#include "Bullet.h"
#include "Constants.h"
#include <vector>

class BulletPool
{
public:
    explicit BulletPool(int size = BULLET_POOL_SIZE)
        : m_poolSize(size)
    {
        m_pool.resize(m_poolSize);  
    }

   
    Bullet* Spawn(Vector2 pos, Vector2 vel,
        float radius, float damage, float lifetime, BulletType type)
    {
        for (int i = 0; i < m_poolSize; ++i)
        {
            if (!m_pool[i].IsActive())
            {
                m_pool[i].Activate(pos, vel, radius, damage, lifetime, type);
                return &m_pool[i];
            }
        }
        return nullptr;   
    }

    void UpdateAll(float dt)
    {
        for (int i = 0; i < m_poolSize; ++i)
            if (m_pool[i].IsActive()) m_pool[i].Update(dt);
    }

    void DrawAll() const
    {
        for (int i = 0; i < m_poolSize; ++i)
            if (m_pool[i].IsActive()) m_pool[i].Draw();
    }

    void DeactivateAll()
    {
        for (int i = 0; i < m_poolSize; ++i)
            m_pool[i].Deactivate();
    }

    int GetActiveCount() const
    {
        int c = 0;
        for (int i = 0; i < m_poolSize; ++i)
            if (m_pool[i].IsActive()) ++c;
        return c;
    }

    int GetPoolSize() const { return m_poolSize; }

    Bullet& operator[](int i) { return m_pool[i]; }
    const Bullet& operator[](int i) const { return m_pool[i]; }

private:
    std::vector<Bullet> m_pool;
    int                 m_poolSize;
};