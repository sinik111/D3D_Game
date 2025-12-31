#pragma once

namespace engine
{
    struct ProjectSettings
    {
        std::vector<std::string> sceneList;

        void Save();
        void Load();
    };
}