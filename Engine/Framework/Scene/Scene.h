#pragma once

#include "Framework/Object/GameObject/GameObject.h"

namespace engine
{
    class Scene
    {
    protected:
        std::string m_name;
        std::vector<std::unique_ptr<GameObject>> m_gameObjects;

    public:
        Scene(const std::string& name);
        virtual ~Scene() = default;

    public:
        virtual void Enter();
        virtual void Exit();

    public:
        GameObject* CreateGameObject(const std::string& name = "GameObject");
    };
}
