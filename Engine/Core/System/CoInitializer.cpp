#include "pch.h"
#include "CoInitializer.h"

namespace engine
{
    CoInitializer::CoInitializer(DWORD dwCoInit)
    {
        HR_CHECK(CoInitializeEx(nullptr, dwCoInit));
    }

    CoInitializer::~CoInitializer()
    {
        CoUninitialize();
    }
}