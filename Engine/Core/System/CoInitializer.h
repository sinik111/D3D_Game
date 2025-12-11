#pragma once

namespace engine
{
    class CoInitializer
    {
    public:
        CoInitializer(DWORD dwCoInit = COINIT_APARTMENTTHREADED);
        ~CoInitializer();
        CoInitializer(const CoInitializer&) = delete;
        CoInitializer& operator=(const CoInitializer&) = delete;
        CoInitializer(CoInitializer&&) = delete;
        CoInitializer& operator=(CoInitializer&&) = delete;
    };
}