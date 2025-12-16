#include "pch.h"
#include "GameObject.h"

#include "Framework/Object/Component/Transform.h"

namespace engine
{
    GameObject::GameObject(const std::string& name)
        : m_name{ name }
    {
        m_transform = AddComponent<Transform>();
    }

    Transform* GameObject::GetTransform() const
    {
        return m_transform;
    }
}