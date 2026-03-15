#pragma once
#include "raylib.h"
#include "Constants.h"

class GameManager
{
private:

     GameManager()
        : m_score(0)
        , m_wave(1)
        , m_kills(0)
        , m_playerHealth(PLAYER_MAX_HEALTH)
        , m_isGameOver(false)
        , m_isPaused(false)
    {
    }

    GameManager(const GameManager&) = delete;
    GameManager& operator=(const GameManager&) = delete;

    int   m_score;
    int   m_wave;
    int   m_kills;
    float m_playerHealth;
    bool  m_isGameOver;
    bool  m_isPaused;

public:
    static GameManager& Instance()
    {
        static GameManager instance;   
        return instance;
    }

     static constexpr int SW = SCREEN_WIDTH;
    static constexpr int SH = SCREEN_HEIGHT;

    void  AddScore(int pts) { m_score += pts; }
    int   GetScore()  const { return m_score; }

    void  AddKill() { m_kills++; }
    int   GetKills()  const { return m_kills; }

    void  NextWave() { m_wave++; }
    int   GetWave()   const { return m_wave; }

    void  DamagePlayer(float dmg)
    {
        m_playerHealth -= dmg;
        if (m_playerHealth < 0.0f)
        {
            m_playerHealth = 0.0f;
            m_isGameOver = true;
        }
    }
    float GetPlayerHealth() const { return m_playerHealth; }
    bool  IsPlayerDead()    const { return m_playerHealth <= 0.0f; }

    bool  IsGameOver()  const { return m_isGameOver; }
    void  SetGameOver(bool v) { m_isGameOver = v; }
    bool  IsPaused()    const { return m_isPaused; }
    void  SetPaused(bool v) { m_isPaused = v; }

  
    void Reset()
    {
        m_score = 0;
        m_wave = 1;
        m_kills = 0;
        m_playerHealth = PLAYER_MAX_HEALTH;
        m_isGameOver = false;
        m_isPaused = false;
    }
};
