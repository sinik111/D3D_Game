#pragma once

#include <cstdint>

#include "Framework/Object/Object.h"

namespace engine
{
    class Component :
        public Object
    {
    private:
        template<std::derived_from<Component> T> friend class System;

        std::int32_t m_systemIndex = -1;
    };
}