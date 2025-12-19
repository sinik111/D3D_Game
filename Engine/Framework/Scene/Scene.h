#pragma once

#include <functional>

#include "Framework/Object/GameObject/GameObject.h"

namespace engine
{
    class Camera;

    class Scene
    {
    protected:
        std::string m_name;
        std::vector<std::unique_ptr<GameObject>> m_gameObjects;
        Camera* m_mainCamera = nullptr;

        // 테스트용으로 enter때 호출할 함수
        std::function<void()> m_onEnter;

    public:
        Scene(const std::string& name, std::function<void()>&& onEnter);

    public:
        void Enter();
        void Exit();

    public:
        GameObject* CreateGameObject(const std::string& name = "GameObject");
        Camera* GetMainCamera() const;
    };
}
