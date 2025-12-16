#pragma once

#include <type_traits>

#include "Framework/Object/Object.h"

namespace engine
{
    template<typename T>
        requires std::is_base_of_v<Object, T>
    class Ptr
    {
    private:
        Handle m_handle;

    public:
        Ptr() = default;
        Ptr(T* object)
        {
            if (object != nullptr)
            {
                m_handle = object->GetHandle();
            }
        }

        Ptr(Handle handle)
            : m_handle{ handle }
        {

        }

        T* operator->() const
        {
            return Get();
        }

        T* Get() const
        {
            Object* object = Object::GetObjectFromHandle(m_handle);
            return static_cast<T*>(object);
        }

        operator bool() const
        {
            return Get() != nullptr;
        }

        bool operator==(const Ptr<T>& other) const
        {
            return m_handle == other.m_handle;
        }

        bool operator!=(const Ptr<T>& other) const
        {
            return m_handle != other.m_handle;
        }
    };
}