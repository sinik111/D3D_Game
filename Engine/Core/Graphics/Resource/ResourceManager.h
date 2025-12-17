#pragma once

#include "Common/Utility/Singleton.h"
#include "Core/Graphics/Device/GraphicsDevice.h"
#include "Core/Graphics/Resource/ResourceKey.h"
#include "Core/Graphics/Resource/VertexBuffer.h"

namespace engine
{
    class GraphicsDevice;
    class VertexBuffer;
    class IndexBuffer;
    class Texture;

    template <typename T>
    concept IsVertex = requires { T::vertexFormat; };

    enum class DefaultTextureType
    {
        White,
        Black,
        Flat,
        Count
    };

    class ResourceManager :
        public Singleton<ResourceManager>
    {
    private:
        std::unordered_map<VertexBufferKey, std::weak_ptr<VertexBuffer>> m_vertexBuffers;
        std::unordered_map<std::string, std::weak_ptr<IndexBuffer>> m_indexBuffers;
        std::unordered_map<std::string, std::weak_ptr<Texture>> m_textures;

        const GraphicsDevice* m_graphicsDevice = nullptr;

    private:
        ResourceManager() = default;
        ~ResourceManager();

    public:
        void Initialize(const GraphicsDevice* graphicsDevice);
        void Cleanup();

    public:
        // VertexBuffer는 버텍스 타입별로 구분이 필요하기 때문에 템플릿으로 만듦
        template <IsVertex T>
        std::shared_ptr<VertexBuffer> GetOrCreateVertexBuffer(const std::string& filePath, const std::vector<T>& vertices)
        {
            VertexBufferKey key{ filePath, T::vertexFormat };
            if (auto find = m_vertexBuffers.find(key); find != m_vertexBuffers.end())
            {
                if (!find->second.expired())
                {
                    return find->second.lock();
                }
            }

            std::shared_ptr<VertexBuffer> vertexBuffer = std::make_shared<VertexBuffer>();
            vertexBuffer->Create(m_graphicsDevice->GetDevice(), vertices);

            m_vertexBuffers[key] = vertexBuffer;

            return vertexBuffer;
        }

        std::shared_ptr<IndexBuffer> GetOrCreateIndexBuffer(const std::string& filePath, const std::vector<DWORD>& indices);
        std::shared_ptr<Texture> GetOrCreateTexture(const std::string& filePath);
        std::shared_ptr<Texture> GetOrCreateTexture(
            const std::string& name,
            UINT width,
            UINT height,
            DXGI_FORMAT format,
            UINT bindFlags);

        std::shared_ptr<Texture> GetOrCreateTexture(
            const std::string& name,
            const D3D11_TEXTURE2D_DESC& desc,
            DXGI_FORMAT srvFormat = DXGI_FORMAT_UNKNOWN,
            DXGI_FORMAT rtvFormat = DXGI_FORMAT_UNKNOWN,
            DXGI_FORMAT dsvFormat = DXGI_FORMAT_UNKNOWN);

        std::shared_ptr<Texture> GetDefaultTexture(DefaultTextureType type);

    private:
        friend class Singleton<ResourceManager>;
    };
}