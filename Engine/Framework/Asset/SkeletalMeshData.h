#pragma once

#include "Framework/Asset/AssetData.h"
#include "Core/Graphics/Data/Vertex.h"

struct aiScene;

namespace engine
{
    class SkeletonData;

    struct SkeletalMeshSection
    {
        std::string name;
        unsigned int m_boneReference = 0;
        unsigned int materialIndex = 0;
        INT vertexOffset;
        UINT indexOffset;
        UINT indexCount;
    };

    class SkeletalMeshData :
        public AssetData
    {
    private:
        std::vector<BoneWeightVertex> m_boneWeightVertices;
        std::vector<CommonVertex> m_vertices; // rigid 용 버텍스
        std::vector<DWORD> m_indices;
        std::vector<SkeletalMeshSection> m_meshSections;
        bool m_isRigid = false;

    public:
        void Create(const aiScene* scene, const std::shared_ptr<SkeletonData>& skeletonData, bool isRigid);

    public:
        const std::vector<BoneWeightVertex>& GetBoneWeightVertices() const;
        const std::vector<CommonVertex>& GetVertices() const;
        const std::vector<DWORD>& GetIndices() const;
        const std::vector<SkeletalMeshSection>& GetMeshSections() const;
        bool IsRigid() const;
    };
}