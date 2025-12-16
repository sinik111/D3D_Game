#include "pch.h"
#include "SceneManager.h"

#include "Framework/Scene/Scene.h"

namespace engine
{
    SceneManager::SceneManager() = default;
    SceneManager::~SceneManager() = default;

    void SceneManager::Shutdown()
    {
        if (m_currentScene != nullptr)
        {
            m_currentScene->Exit();
        }

        m_scenes.clear();
    }

    void SceneManager::CreateScene(const std::string& name)
    {
        if (m_scenes.find(name) == m_scenes.end())
        {
            m_scenes.emplace(name, std::make_unique<Scene>(name));
        }
    }

    void SceneManager::ChangeScene(const std::string& name)
    {
        if (auto it = m_scenes.find(name); it != m_scenes.end())
        {
            m_nextScene = it->second.get();
        }
    }

    void SceneManager::CheckSceneChanged()
    {
        if (m_nextScene != nullptr)
        {
            if (m_currentScene != nullptr)
            {
                m_currentScene->Exit();
            }

            m_currentScene = m_nextScene;

            m_nextScene = nullptr;

            m_currentScene->Enter();
        }
    }
}