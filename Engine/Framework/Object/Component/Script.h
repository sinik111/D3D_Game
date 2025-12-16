#pragma once

#include "Framework/Object/Component/Component.h"

namespace engine
{
    enum class ScriptEvent
    {
        Initialize,
        Start,
        Update,
        FixedUpdate,
        LateUpdate,
        Count
    };

    class Script :
        public Component
    {
    private:
        friend class ScriptSystem;

        std::array<std::int32_t, static_cast<size_t>(ScriptEvent::Count)> m_systemIndices;

    public:
        Script();
        virtual ~Script();

    public:
        virtual void Initialize();
        virtual void Start();
        virtual void Update();
    };
}