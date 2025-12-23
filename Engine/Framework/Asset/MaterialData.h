#pragma once

#include "Framework/Asset/AssetData.h"

struct aiScene;

namespace engine
{
    enum class MaterialKey : std::uint64_t
    {
        BASE_COLOR_TEXTURE         = 1ULL << 0,
        NORMAL_TEXTURE             = 1ULL << 1,
        EMISSIVE_TEXTURE           = 1ULL << 2,
        METALNESS_TEXTURE          = 1ULL << 3,
        ROUGHNESS_TEXTURE          = 1ULL << 4,
        AMBIENT_OCCLUSION_TEXTURE  = 1ULL << 5,

        BASE_COLOR                 = 1ULL << 6,
        EMISSIVE_COLOR             = 1ULL << 7,

        ROUGHNESS                  = 1ULL << 8,
        METALNESS                  = 1ULL << 9,
        EMISSIVE_INTENSITY         = 1ULL << 10,
        AMBIENT_OCCLUSION          = 1ULL << 11,
    };

    struct Material
    {
        std::unordered_map<MaterialKey, std::string> texturePaths;
        std::unordered_map<MaterialKey, Vector4> vectorValues;
        std::unordered_map<MaterialKey, float> scalarValues;
        std::uint64_t materialFlags = 0;
    };

    class MaterialData :
        public AssetData
    {
    private:
        std::vector<Material> m_materials;

    public:
        void Create();
        void Create(const std::string& filePath);
        void Create(const aiScene* scene);

    public:
        const std::vector<Material>& GetMaterials() const;
    };
}