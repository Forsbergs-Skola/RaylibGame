#pragma once
#include "Enemy.h"
#include "EnemyFactory.h"
#include "Constants.h"
#include <vector>

// ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//  EnemyPool --- Object Pool for enemies (same pattern as BulletPool).
//  Works with EnemyFactory: the pool finds a free slot, the factory
//  configures it, then Activate() positions it in the world.
// ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class EnemyPool
{
public:
    explicit EnemyPool(int size = ENEMY_POOL_SIZE)
        : m_poolSize(size)
    {
        m_pool.resize(m_poolSize);
    }

    // Factory + Pool: configure the free enemy then activate it
    Enemy* Spawn(EnemyType type, Vector2 pos)
    {
        for (int i = 0; i < m_poolSize; ++i)
        {
            if (!m_pool[i].IsActive())
            {
                EnemyFactory::Configure(&m_pool[i], type);
                m_pool[i].Activate(pos);
                return &m_pool[i];
            }
        }
        return nullptr;   // pool full
    }

    void UpdateAll(float dt, Vector2 playerPos)
    {
        for (int i = 0; i < m_poolSize; ++i)
            if (m_pool[i].IsActive()) m_pool[i].Update(dt, playerPos);
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

    Enemy& operator[](int i) { return m_pool[i]; }
    const Enemy& operator[](int i) const { return m_pool[i]; }

private:
    std::vector<Enemy> m_pool;
    int                m_poolSize;
};