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

        // 테스트용으로 enter때 호출할 함수
        std::function<void()> m_onEnter;

    public:
        Scene();

    public:
        void Save();
        void SaveToJson(json& outJson);

        void Load();
        void LoadFromJson(const json& inJson);

    public:
        GameObject* CreateGameObject(const std::string& name = "GameObject");
        Camera* GetMainCamera() const;
        const std::vector<std::unique_ptr<GameObject>>& GetGameObjects() const;
        const std::string& GetName() const;

        void SetName(std::string_view name);

        GameObject* FindGameObject(const std::string& name);

        void Reset();
    };
}
