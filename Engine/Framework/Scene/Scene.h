#pragma once

#include <functional>

#include "Framework/Object/GameObject/GameObject.h"

namespace engine
{
    class Scene
    {
    protected:
        std::string m_name;
        std::vector<std::unique_ptr<GameObject>> m_gameObjects;

        // 테스트용으로 enter때 호출할 함수
        std::function<void()> m_onEnter;

    public:
        Scene(const std::string& name, std::function<void()>&& onEnter);
        virtual ~Scene() = default;

    public:
        virtual void Enter();
        virtual void Exit();

    public:
        GameObject* CreateGameObject(const std::string& name = "GameObject");
    };
}
