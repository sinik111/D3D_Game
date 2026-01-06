#pragma once

#include "Framework/Asset/AssetData.h"
#include "Core/Graphics/Data/Vertex.h"

struct aiScene;

namespace engine
{
    class SimpleMeshData :
        public AssetData
    {
    private:
        std::vector<PositionTexCoordVertex> m_vertices;
        std::vector<WORD> m_indices;

    public:
        void Create(const std::string& filePath);
        void Create(const aiScene* scene);

    public:
        const std::vector<PositionTexCoordVertex>& GetVertices() const;
        const std::vector<WORD>& GetIndices() const;

    };
}