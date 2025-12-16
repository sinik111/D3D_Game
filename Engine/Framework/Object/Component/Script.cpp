#include "pch.h"
#include "Script.h"

#include "Framework/System/SystemManager.h"
#include "Framework/System/ScriptSystem.h"

namespace engine
{
    ScriptBase::ScriptBase()
    {
        m_systemIndices.fill(-1);
    }

    ScriptBase::~ScriptBase()
    {
        SystemManager::Get().Script().Unregister(this);
    }
    void ScriptBase::RegisterScript(std::uint32_t eventFlags)
    {
        SystemManager::Get().Script().Register(this, eventFlags);
    }

    void ScriptBase::Initialize()
    {
    }
    void ScriptBase::Start()
    {
    }
    void ScriptBase::Update()
    {
    }
}