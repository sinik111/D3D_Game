#include "pch.h"
#include "Scene.h"

namespace engine
{
    Scene::Scene(const std::string& name)
        : m_name{ name }
    {

    }

    void Scene::Enter()
    {
        LOG_PRINT("{} scene enter", m_name);
    }

    void Scene::Exit()
    {
        LOG_PRINT("{} scene exit", m_name);
        m_gameObjects.clear();
    }

    GameObject* Scene::CreateGameObject(const std::string& name)
    {
        m_gameObjects.push_back(std::make_unique<GameObject>());

        return m_gameObjects.back().get();
    }
}