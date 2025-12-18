#include "pch.h"
#include "ResourceManager.h"

#include "Core/Graphics/Resource/IndexBuffer.h"
#include "Core/Graphics/Resource/Texture.h"
#include "Core/Graphics/Resource/ConstantBuffer.h"
#include "Core/Graphics/Resource/VertexShader.h"
#include "Core/Graphics/Resource/PixelShader.h"

namespace engine
{
    namespace
    {
        // 프로그램 종료시까지 지워지면 안돼서 shared_ptr로 보관함
        std::array<std::shared_ptr<Texture>, static_cast<size_t>(DefaultTextureType::Count)> g_defaultTextures;
    }

    ResourceManager::~ResourceManager() = default;

    void ResourceManager::Initialize()
    {
        // 1 * 1 크기의 기본 텍스쳐 생성
        auto white = std::make_shared<Texture>();
        white->Create(GraphicsDevice::Get().GetDevice(), std::array<unsigned char, 4>{ 255, 255, 255, 255 });
        g_defaultTextures[static_cast<size_t>(DefaultTextureType::White)] = std::move(white);

        auto black = std::make_shared<Texture>();
        black->Create(GraphicsDevice::Get().GetDevice(), std::array<unsigned char, 4>{ 0, 0, 0, 255 });
        g_defaultTextures[static_cast<size_t>(DefaultTextureType::Black)] = std::move(black);

        auto flat = std::make_shared<Texture>();
        flat->Create(GraphicsDevice::Get().GetDevice(), std::array<unsigned char, 4>{ 128, 128, 255, 255 });
        g_defaultTextures[static_cast<size_t>(DefaultTextureType::Flat)] = std::move(flat);
    }

    void ResourceManager::Cleanup()
    {
        g_defaultTextures.fill(nullptr); // 직접 use_count 감소
    }

    std::shared_ptr<IndexBuffer> ResourceManager::GetOrCreateIndexBuffer(const std::string& filePath, const std::vector<DWORD>& indices)
    {
        if (auto find = m_indexBuffers.find(filePath); find != m_indexBuffers.end())
        {
            if (!find->second.expired())
            {
                return find->second.lock();
            }
        }

        auto indexBuffer = std::make_shared<IndexBuffer>();
        indexBuffer->Create(indices);

        m_indexBuffers[filePath] = indexBuffer;

        return indexBuffer;
    }

    std::shared_ptr<Texture> ResourceManager::GetOrCreateTexture(const std::string& filePath)
    {
        if (auto find = m_textures.find(filePath); find != m_textures.end())
        {
            if (!find->second.expired())
            {
                return find->second.lock();
            }
        }

        auto texture = std::make_shared<Texture>();
        texture->Create(filePath);

        m_textures[filePath] = texture;

        return texture;
    }

    std::shared_ptr<Texture> ResourceManager::GetOrCreateTexture(
        const std::string& name,
        UINT width,
        UINT height,
        DXGI_FORMAT format,
        UINT bindFlags)
    {
        if (auto find = m_textures.find(name); find != m_textures.end())
        {
            if (!find->second.expired())
            {
                return find->second.lock();
            }
        }

        auto texture = std::make_shared<Texture>();
        texture->Create(GraphicsDevice::Get().GetDevice(), width, height, format, bindFlags);

        m_textures[name] = texture;

        return texture;
    }

    std::shared_ptr<Texture> ResourceManager::GetOrCreateTexture(
        const std::string& name,
        const D3D11_TEXTURE2D_DESC& desc,
        DXGI_FORMAT srvFormat,
        DXGI_FORMAT rtvFormat,
        DXGI_FORMAT dsvFormat)
    {
        if (auto find = m_textures.find(name); find != m_textures.end())
        {
            if (!find->second.expired())
            {
                return find->second.lock();
            }
        }

        auto texture = std::make_shared<Texture>();
        texture->Create(GraphicsDevice::Get().GetDevice(), desc, srvFormat, rtvFormat, dsvFormat);

        m_textures[name] = texture;

        return texture;
    }

    std::shared_ptr<Texture> ResourceManager::GetDefaultTexture(DefaultTextureType type)
    {
        return g_defaultTextures[static_cast<size_t>(type)];
    }

    std::shared_ptr<ConstantBuffer> ResourceManager::GetOrCreateConstantBuffer(const std::string& name, UINT byteWidth)
    {
        if (auto find = m_constantBuffers.find(name); find != m_constantBuffers.end())
        {
            if (!find->second.expired())
            {
                return find->second.lock();
            }
        }

        auto constantBuffer = std::make_shared<ConstantBuffer>();
        constantBuffer->Create(byteWidth);

        m_constantBuffers[name] = constantBuffer;

        return constantBuffer;
    }

    std::shared_ptr<VertexShader> ResourceManager::GetOrCreateVertexShader(const std::string& filePath)
    {
        if (auto find = m_vertexShaders.find(filePath); find != m_vertexShaders.end())
        {
            if (!find->second.expired())
            {
                return find->second.lock();
            }
        }

        auto vertexShader = std::make_shared<VertexShader>();
        vertexShader->Create(filePath);

        m_vertexShaders[filePath] = vertexShader;

        return vertexShader;
    }

    std::shared_ptr<PixelShader> ResourceManager::GetOrCreatePixelShader(const std::string& filePath)
    {
        if (auto find = m_pixelShaders.find(filePath); find != m_pixelShaders.end())
        {
            if (!find->second.expired())
            {
                return find->second.lock();
            }
        }

        auto pixelShader = std::make_shared<PixelShader>();
        pixelShader->Create(filePath);

        m_pixelShaders[filePath] = pixelShader;

        return pixelShader;
    }
}