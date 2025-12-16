#pragma once

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
        friend class Singleton<SceneManager>;

    public:
        void Shutdown();

    public:
        void CreateScene(const std::string& name);
        void ChangeScene(const std::string& name);
        void CheckSceneChanged();
        Scene* GetCurrentScene() const;
    };
}