#include "EnginePCH.h"
#include "IndexBuffer.h"

#include "Core/Graphics/Device/GraphicsDevice.h"

namespace engine
{
    void IndexBuffer::Create(const std::vector<DWORD>& indices)
    {
        D3D11_BUFFER_DESC indexBufferDesc{};
        indexBufferDesc.ByteWidth = static_cast<UINT>(sizeof(DWORD) * indices.size());
        indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
        indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;

        D3D11_SUBRESOURCE_DATA indexBufferData{};
        indexBufferData.pSysMem = indices.data();

        HR_CHECK(GraphicsDevice::Get().GetDevice()->CreateBuffer(&indexBufferDesc, &indexBufferData, &m_buffer));
    }

    void IndexBuffer::Create(const std::vector<WORD>& indices)
    {
        D3D11_BUFFER_DESC indexBufferDesc{};
        indexBufferDesc.ByteWidth = static_cast<UINT>(sizeof(WORD) * indices.size());
        indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
        indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;

        D3D11_SUBRESOURCE_DATA indexBufferData{};
        indexBufferData.pSysMem = indices.data();

        HR_CHECK(GraphicsDevice::Get().GetDevice()->CreateBuffer(&indexBufferDesc, &indexBufferData, &m_buffer));
    }

    const Microsoft::WRL::ComPtr<ID3D11Buffer>& IndexBuffer::GetBuffer() const
    {
        return m_buffer;
    }

    ID3D11Buffer* IndexBuffer::GetRawBuffer() const
    {
        return m_buffer.Get();
    }
}