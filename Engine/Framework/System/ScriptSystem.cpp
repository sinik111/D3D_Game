#include "pch.h"
#include "ScriptSystem.h"

namespace engine
{
    void ScriptSystem::Unregister(Script* script)
    {
        RemoveScript(m_startScripts, script, ScriptEvent::Start);
        RemoveScript(m_updateScripts, script, ScriptEvent::Update);
    }

    void ScriptSystem::CallStart()
    {
        for (auto& script : m_startScripts)
        {
            script->Start();
            script->m_systemIndices[static_cast<size_t>(ScriptEvent::Start)] = -1;
        }

        m_startScripts.clear();
    }

    void ScriptSystem::CallUpdate()
    {
        for (auto& script : m_updateScripts)
        {
            script->Update();
        }
    }

    void ScriptSystem::AddScript(std::vector<Script*>& v, Script* script, ScriptEvent type)
    {
        if (script->m_systemIndices[static_cast<size_t>(type)] != -1)
        {
            return; // 이미 등록됨
        }

        script->m_systemIndices[static_cast<size_t>(type)] = static_cast<std::int32_t>(v.size());
        v.push_back(script);
    }

    void ScriptSystem::RemoveScript(std::vector<Script*>& v, Script* script, ScriptEvent type)
    {
        std::int32_t index = script->m_systemIndices[static_cast<size_t>(type)];
        if (index < 0)
        {
            return; // 등록 안됨
        }

        Script* back = v.back();
        v[index] = back;
        v.pop_back();
        
        back->m_systemIndices[(int)type] = index;
        script->m_systemIndices[(int)type] = -1;
    }
}