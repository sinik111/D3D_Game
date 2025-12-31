#pragma once

#include <type_traits>
#include <cstddef>
#include <new>

namespace engine
{
    template <typename T, size_t Capacity>
    requires std::is_class_v<T> && (Capacity > 0)
    class StaticMemoryPool
    {
    private:
        union Slot
        {
            alignas(T) std::byte element[sizeof(T)];
            Slot* next;
        };

    private:
        alignas(Slot) std::byte m_buffer[Capacity * sizeof(Slot)]{};
        Slot* m_freeList = nullptr;

    public:
        StaticMemoryPool()
        {
            for (size_t i = 0; i < Capacity - 1; ++i)
            {
                GetSlot(i)->next = GetSlot(i + 1);
            }

            GetSlot(Capacity - 1)->next = nullptr;
            m_freeList = GetSlot(0);
        }

        StaticMemoryPool(const StaticMemoryPool&) = delete;
        StaticMemoryPool operator=(const StaticMemoryPool&) = delete;
        StaticMemoryPool(StaticMemoryPool&&) = delete;
        StaticMemoryPool operator=(StaticMemoryPool&&) = delete;
        ~StaticMemoryPool() = default;

        void* Allocate(size_t size)
        {
            if (size != sizeof(T)) [[unlikely]]
            {
                return ::operator new(size);
            }

            if (m_freeList == nullptr) [[unlikely]]
            {
                return ::operator new(size);
            }

            Slot* slot = m_freeList;
            m_freeList = slot->next;
            return slot;
        }

        void Deallocate(void* ptr)
        {
            if (!IsFromPool(ptr)) [[unlikely]]
            {
                ::operator delete(ptr);
                return;
            }

            Slot* slot = static_cast<Slot*>(ptr);
            slot->next = m_freeList;
            m_freeList = slot;
        }

    private:
        Slot* GetSlot(size_t index)
        {
            return reinterpret_cast<Slot*>(&m_buffer[index * sizeof(Slot)]);
        }

        bool IsFromPool(void* ptr) const
        {
            const std::byte* bytePtr = static_cast<const std::byte*>(ptr);
            const std::byte* start = reinterpret_cast<const std::byte*>(m_buffer);
            const std::byte* end = start + sizeof(m_buffer);

            return bytePtr >= start && bytePtr < end;
        }
    };
}