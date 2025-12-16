#include "pch.h"
#include "Script.h"

#include "Framework/System/SystemManager.h"
#include "Framework/System/ScriptSystem.h"

namespace engine
{
    Script::Script()
    {
        SystemManager::Get().Script().Register(this);
    }

    Script::~Script()
    {
        SystemManager::Get().Script().Unregister(this);
    }

    void Script::Initialize()
    {
    }

    void Script::Start()
    {
    }

    void Script::Update()
    {
    }
}