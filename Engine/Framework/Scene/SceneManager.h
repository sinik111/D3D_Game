#pragma once

namespace engine
{
    class Scene;

    class SceneManager
    {
    private:
        std::unordered_map<std::string, std::unique_ptr<Scene>> m_scenes;

        Scene* m_currentScene = nullptr;
        Scene* m_nextScene = nullptr;

    private:
        SceneManager() = default;
        ~SceneManager() = default;
        SceneManager(const SceneManager&) = delete;
        SceneManager& operator=(const SceneManager&) = delete;
        SceneManager(SceneManager&&) = delete;
        SceneManager& operator=(SceneManager&&) = delete;

    public:
        static SceneManager& Get();

    public:
        void Shutdown();

    public:
        void CreateScene(std::string_view name);
        void ChangeScene(std::string_view name);
        void CheckSceneChanged();
    };
}