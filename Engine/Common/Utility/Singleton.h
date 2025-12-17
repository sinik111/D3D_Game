#pragma once

namespace engine
{
    template <typename T>
    class Singleton
    {
    protected:
        Singleton() = default;
        virtual ~Singleton() = default;
        Singleton(const Singleton&) = delete;
        Singleton& operator=(const Singleton&) = delete;

    public:
        static T& Get()
        {
            static T s_instance;
            return s_instance;
        }
    };
}