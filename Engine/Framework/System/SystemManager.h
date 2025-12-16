#pragma once

namespace engine
{
    class ScriptSystem;

    class SystemManager :
        public Singleton<SystemManager>
    {
    private:
        std::unique_ptr<ScriptSystem> m_scriptSystem;

    private:
        friend class Singleton<SystemManager>;
        SystemManager();
        ~SystemManager();

    public:
        ScriptSystem& Script() const;
    };
}
