#include "pch.h"
#include "SystemManager.h"

#include "Framework/System/ScriptSystem.h"

namespace engine
{
    SystemManager::SystemManager()
        : m_scriptSystem{ std::make_unique<ScriptSystem>() }
    {
    }

    SystemManager::~SystemManager()
    {
        int a = 0;
    }

    ScriptSystem& SystemManager::Script() const
    {
        return *m_scriptSystem.get();
    }
}