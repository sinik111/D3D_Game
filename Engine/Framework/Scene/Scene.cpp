#include "pch.h"
#include "Scene.h"

#include <functional>
#include <fstream>

#include "Common/Utility/JsonHelper.h"
#include "Framework/Object/Component/Camera.h"
#include "Framework/Object/Component/Transform.h"
#include "Framework/System/SystemManager.h"
#include "Framework/System/CameraSystem.h"

namespace engine
{
    Scene::Scene()
    {
        auto gameObject = CreateGameObject("MainCamera");
        gameObject->AddComponent<Camera>();
    }

    void Scene::Save()
    {
        json root;
        root["Name"] = m_name;
        root["NumGameObjects"] = m_gameObjects.size();
        root["GameObjects"] = json::array();

        std::vector<GameObject*> sortedList;
        sortedList.reserve(m_gameObjects.size());

        std::function<void(Transform*)> traverse = [&](Transform* tr)
            {
                sortedList.push_back(tr->GetGameObject());
                for (auto child : tr->GetChildren())
                {
                    traverse(child);
                }
            };

        for (const auto& go : m_gameObjects)
        {
            if (go->GetTransform()->GetParent() == nullptr)
            {
                traverse(go->GetTransform());
            }
        }

        std::unordered_map<GameObject*, int> ptrToId;
        int currentId = 0;
        for (auto go : sortedList)
        {
            ptrToId[go] = currentId++;
        }

        for (auto go : sortedList)
        {
            json goJson;
            goJson["ID"] = ptrToId[go];

            Transform* parent = go->GetTransform()->GetParent();
            if (parent != nullptr)
            {
                if (auto iter = ptrToId.find(parent->GetGameObject()); iter != ptrToId.end())
                {
                    goJson["ParentID"] = iter->second;
                }
            }

            go->Save(goJson);
            root["GameObjects"].push_back(goJson);
        }

        std::filesystem::path path{ "Resource/Scene" };
        path /= (m_name + ".json");

        if (path.has_parent_path())
        {
            std::error_code ec;
            if (!std::filesystem::create_directories(path.parent_path(), ec))
            {
                FATAL_CHECK(ec.value() == 0, ec.message());
            }
        }

        std::ofstream o(path);
        if (o.is_open())
        {
            o << std::setw(4) << root << std::endl;
        }
    }

    void Scene::SaveToJson(json& outJson)
    {
        outJson["Name"] = m_name;
        outJson["NumGameObjects"] = m_gameObjects.size();
        outJson["GameObjects"] = json::array();

        std::vector<GameObject*> sortedList;
        sortedList.reserve(m_gameObjects.size());

        std::function<void(Transform*)> traverse = [&](Transform* tr)
            {
                sortedList.push_back(tr->GetGameObject());
                for (auto child : tr->GetChildren())
                {
                    traverse(child);
                }
            };

        for (const auto& go : m_gameObjects)
        {
            if (go->GetTransform()->GetParent() == nullptr)
            {
                traverse(go->GetTransform());
            }
        }

        std::unordered_map<GameObject*, int> ptrToId;
        int currentId = 0;
        for (auto go : sortedList)
        {
            ptrToId[go] = currentId++;
        }

        for (auto go : sortedList)
        {
            json goJson;
            goJson["ID"] = ptrToId[go];

            Transform* parent = go->GetTransform()->GetParent();
            if (parent != nullptr)
            {
                if (auto iter = ptrToId.find(parent->GetGameObject()); iter != ptrToId.end())
                {
                    goJson["ParentID"] = iter->second;
                }
            }

            go->Save(goJson);
            outJson["GameObjects"].push_back(goJson);
        }
    }

    void Scene::Load()
    {
        std::filesystem::path path{ "Resource/Scene" };
        path /= (m_name + ".json");

        std::ifstream i(path);
        if (!i.is_open())
        {
            FATAL_CHECK(false, path.string());
            return;
        }

        json root;
        i >> root;

        m_gameObjects.clear();

        JsonGet(root, "Name", m_name);
        size_t numGameObjects;
        JsonGet(root, "NumGameObjects", numGameObjects);


        std::vector<GameObject*> idToPtr(numGameObjects + 1);
        std::vector<std::pair<int, int>> parentLinks;

        JsonArrayForEach(root, "GameObjects", [&](const json& goJson)
            {
                std::string name = goJson.value("Name", "GameObject");
                GameObject* go = CreateGameObject(name);

                int id = goJson.value("ID", -1);
                if (id >= 0 && id < idToPtr.size())
                {
                    idToPtr[id] = go;
                }

                go->Load(goJson);

                int parentId = -1;
                JsonGet(goJson, "ParentID", parentId);
                if (parentId != -1)
                {
                    parentLinks.push_back({ id, parentId });
                }
            });

        for (const auto& link : parentLinks)
        {
            int childId = link.first;
            int parentId = link.second;

            if (childId < idToPtr.size() && parentId < idToPtr.size())
            {
                GameObject* c = idToPtr[childId];
                GameObject* p = idToPtr[parentId];

                if (c != nullptr && p != nullptr)
                {
                    c->GetTransform()->SetParent(p->GetTransform());
                }
            }
        }
    }

    void Scene::LoadFromJson(const json& inJson)
    {
        m_gameObjects.clear();

        JsonGet(inJson, "Name", m_name);
        size_t numGameObjects;
        JsonGet(inJson, "NumGameObjects", numGameObjects);


        std::vector<GameObject*> idToPtr(numGameObjects + 1);
        std::vector<std::pair<int, int>> parentLinks;

        JsonArrayForEach(inJson, "GameObjects", [&](const json& goJson)
            {
                std::string name = goJson.value("Name", "GameObject");
                GameObject* go = CreateGameObject(name);

                int id = goJson.value("ID", -1);
                if (id >= 0 && id < idToPtr.size())
                {
                    idToPtr[id] = go;
                }

                go->Load(goJson);

                int parentId = -1;
                JsonGet(goJson, "ParentID", parentId);
                if (parentId != -1)
                {
                    parentLinks.push_back({ id, parentId });
                }
            });

        for (const auto& link : parentLinks)
        {
            int childId = link.first;
            int parentId = link.second;

            if (childId < idToPtr.size() && parentId < idToPtr.size())
            {
                GameObject* c = idToPtr[childId];
                GameObject* p = idToPtr[parentId];

                if (c != nullptr && p != nullptr)
                {
                    c->GetTransform()->SetParent(p->GetTransform());
                }
            }
        }
    }

    GameObject* Scene::CreateGameObject(const std::string& name)
    {
        m_gameObjects.push_back(std::make_unique<GameObject>());
        m_gameObjects.back().get()->SetName(name);

        return m_gameObjects.back().get();
    }

    Camera* Scene::GetMainCamera() const
    {
        return SystemManager::Get().GetCameraSystem().GetMainCamera();
    }

    const std::vector<std::unique_ptr<GameObject>>& Scene::GetGameObjects() const
    {
        return m_gameObjects;
    }

    const std::string& Scene::GetName() const
    {
        return m_name;
    }

    void Scene::SetName(std::string_view name)
    {
        m_name = name;
    }

    GameObject* Scene::FindGameObject(const std::string& name)
    {
        for (const auto& gameObject : m_gameObjects)
        {
            if (gameObject->GetName() == name)
            {
                return gameObject.get();
            }
        }

        return nullptr;
    }

    void Scene::Reset()
    {
        m_gameObjects.clear();

        auto gameObject = CreateGameObject("MainCamera");
        gameObject->AddComponent<Camera>();
    }
}