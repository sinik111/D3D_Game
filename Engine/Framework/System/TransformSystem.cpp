#include "pch.h"
#include "TransformSystem.h"

namespace engine
{
    void TransformSystem::UnmarkDirtyThisFrame()
    {
        for (auto& transform : m_components)
        {
            transform->UnmarkDirtyThisFrame();
        }
    }
}