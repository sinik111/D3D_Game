#pragma once

#include "Framework/System/System.h"
#include "Framework/Object/Component/Transform.h"

namespace engine
{
    class TransformSystem :
        public System<Transform>
    {
    public:
        void UnmarkDirtyThisFrame();
    };
}