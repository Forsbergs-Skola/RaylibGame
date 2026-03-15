#pragma once
#include "GameState.h"
#include <map>
#include <string>

class StateManager
{
public:
    StateManager() : m_current(nullptr), m_next(nullptr) {}

    ~StateManager()
    {
        for (auto it = m_states.begin(); it != m_states.end(); ++it)
            delete it->second;
    }

    void AddState(const std::string& name, GameState* state)
    {
        m_states[name] = state;
    }

    void ChangeState(const std::string& name)
    {
        auto it = m_states.find(name);
        if (it != m_states.end())
            m_next = it->second;
    }

    void Update(float dt)
    {
        // ------ Handle pending transition ------------------------------------------------------------------------------------------------------------
        if (m_next && m_next != m_current)
        {
            if (m_current) m_current->Exit();
            m_current = m_next;
            m_next = nullptr;
            m_current->Enter();
        }

        // ------ Update active state ------------------------------------------------------------------------------------------------------------------------------
        if (!m_current) return;

        const char* req = m_current->Update(dt);
        if (req && req[0] != '\0')
            ChangeState(req);
    }

    void Draw() const
    {
        if (m_current) m_current->Draw();
    }

    const char* GetCurrentName() const
    {
        return m_current ? m_current->GetName() : "None";
    }

private:
    std::map<std::string, GameState*> m_states;
    GameState* m_current;
    GameState* m_next;
};