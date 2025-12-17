#pragma once

#include <functional>

#include "Common/Utility/Singleton.h"

namespace engine
{
    class Scene;

    class SceneManager :
        public Singleton<SceneManager>
    {
    private:
        std::unordered_map<std::string, std::unique_ptr<Scene>> m_scenes;

        Scene* m_currentScene = nullptr;
        Scene* m_nextScene = nullptr;

    private:
        SceneManager();
        ~SceneManager();

    public:
        void Shutdown();

    public:
        void CreateScene(const std::string& name, std::function<void()>&& onEnter);
        void ChangeScene(const std::string& name);
        void CheckSceneChanged();
        Scene* GetCurrentScene() const;

    private:
        friend class Singleton<SceneManager>;
    };
}