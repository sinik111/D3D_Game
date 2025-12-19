#include "pch.h"
#include "SystemManager.h"

#include "Framework/System/ScriptSystem.h"
#include "Framework/System/TransformSystem.h"

namespace engine
{
    SystemManager::SystemManager()
        : m_scriptSystem{ std::make_unique<ScriptSystem>() },
        m_transformSystem{ std::make_unique<TransformSystem>() }
    {
    }

    SystemManager::~SystemManager() = default;

    ScriptSystem& SystemManager::Script() const
    {
        return *m_scriptSystem.get();
    }

    TransformSystem& SystemManager::Transform() const
    {
        return *m_transformSystem.get();
    }
}