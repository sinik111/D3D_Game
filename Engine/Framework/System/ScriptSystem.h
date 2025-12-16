#pragma once

#include "Framework/System/System.h"
#include "Framework/Object/Component/Script.h"

namespace engine
{
    class ScriptSystem :
        public System<ScriptBase>
    {
    private:
        std::vector<ScriptBase*> m_startScripts;
        std::vector<ScriptBase*> m_updateScripts;

    public:
        void Register(ScriptBase* script, std::uint32_t eventFlags);
        virtual void Unregister(ScriptBase* script) override;

    public:
        void CallStart();
        void CallUpdate();

    private:
        void AddScript(std::vector<ScriptBase*>& v, ScriptBase* script, ScriptEvent type);
        void RemoveScript(std::vector<ScriptBase*>& v, ScriptBase* script, ScriptEvent type);
    };
}