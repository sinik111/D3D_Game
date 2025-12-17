#pragma once

#include "Common/Utility/Singleton.h"

namespace engine
{
    class ScriptSystem;

    class SystemManager :
        public Singleton<SystemManager>
    {
    private:
        std::unique_ptr<ScriptSystem> m_scriptSystem;

    private:
        SystemManager();
        ~SystemManager();

    public:
        ScriptSystem& Script() const;

    private:
        friend class Singleton<SystemManager>;
    };
}
