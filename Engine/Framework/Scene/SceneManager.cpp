#include "pch.h"
#include "SceneManager.h"

#include "Framework/Scene/Scene.h"

namespace engine
{
    SceneManager::SceneManager() = default;
    SceneManager::~SceneManager() = default;

    void SceneManager::Initialize()
    {
        m_activeScene = std::make_unique<Scene>();
        m_activeScene->SetName("SampleScene");
    }

    void SceneManager::Shutdown()
    {
        m_activeScene.reset();
    }

    void SceneManager::ChangeScene(const std::string& name)
    {
        m_nextSceneName = name;
        m_isSceneChanged = true;
    }

    void SceneManager::CheckSceneChanged()
    {
        if (m_isSceneChanged)
        {
            m_activeScene->SetName(m_nextSceneName);
            m_activeScene->Load();

            m_isSceneChanged = false;
        }
    }

    Scene* SceneManager::GetScene() const
    {
        return m_activeScene.get();
    }
}