#pragma once

#include "Framework/GameObject/GameObject.h"

namespace engine
{
    class Scene
    {
    protected:
        std::vector<std::unique_ptr<GameObject>> m_gameObjects;

    public:
        virtual ~Scene() = default;

    public:
        virtual void Enter() = 0;
        virtual void Exit();

    public:
        GameObject* CreateGameObject(std::string_view name = "GameObject");
    };
}
