#pragma once

#include <cstdint>

namespace engine
{
    struct Handle
    {
        std::uint32_t index = 0;
        std::uint32_t generation = 0;

        bool operator==(const Handle& other) const
        {
            return (index == other.index) && (generation == other.generation);
        }

        bool operator!=(const Handle& other) const
        {
            return !(*this == other);
        }

        bool IsValid() const
        {
            return (index != 0) || (generation != 0);
        }
    };
}