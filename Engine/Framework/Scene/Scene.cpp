#include "pch.h"
#include "Scene.h"

#include "Framework/Object/Component/Camera.h"

namespace engine
{
    Scene::Scene(const std::string& name, std::function<void()>&& onEnter)
        : m_name{ name }, m_onEnter{ std::move(onEnter) }
    {

    }

    void Scene::Enter()
    {
        LOG_PRINT("{} scene enter", m_name);

        auto gameObject = CreateGameObject("MainCamera");
        m_mainCamera = gameObject->AddComponent<Camera>();

        auto viewport = GraphicsDevice::Get().GetViewport();

        m_mainCamera->SetWidth(viewport.Width);
        m_mainCamera->SetHeight(viewport.Height);

        m_onEnter();
    }

    void Scene::Exit()
    {
        LOG_PRINT("{} scene exit", m_name);
        m_gameObjects.clear();
        m_mainCamera = nullptr;
    }

    GameObject* Scene::CreateGameObject(const std::string& name)
    {
        m_gameObjects.push_back(std::make_unique<GameObject>());

        return m_gameObjects.back().get();
    }

    Camera* Scene::GetMainCamera() const
    {
        return m_mainCamera;
    }
}