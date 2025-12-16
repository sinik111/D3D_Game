#pragma once

#include "Framework/System/System.h"
#include "Framework/Object/Component/Script.h"

namespace engine
{
    class ScriptSystem :
        public System<Script>
    {
    private:
        std::vector<Script*> m_startScripts;
        std::vector<Script*> m_updateScripts;

    public:
        template<std::derived_from<Script> T>
        void Register(T* script)
        {
            // 컴파일 타임에 해당 이벤트 함수들 구현되었는지 확인 후
            // 구현 된 스크립트들만 각각의 벡터에 추가함

            if constexpr (IsFuncOverridden(&Script::Start, &T::Start))
            {
                AddScript(m_startScripts, script, ScriptEvent::Start);
            }

            if constexpr (IsFuncOverridden(&Script::Update, &T::Update))
            {
                AddScript(m_updateScripts, script, ScriptEvent::Update);
            }
        }
        virtual void Unregister(Script* script) override;

    public:
        void CallStart();
        void CallUpdate();

    private:
        template <typename Base, typename Derived>
        consteval bool IsFuncOverridden(void (Base::* baseFunc)(), void (Derived::* derivedFunc)())
        {
            return baseFunc != static_cast<void (Base::*)()>(derivedFunc);
        }

        void AddScript(std::vector<Script*>& v, Script* script, ScriptEvent type);
        void RemoveScript(std::vector<Script*>& v, Script* script, ScriptEvent type);
    };
}