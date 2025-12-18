#include "pch.h"
#include "ConstantBuffer.h"

namespace engine
{
    void ConstantBuffer::Create(UINT byteWidth)
    {
        D3D11_BUFFER_DESC constantBufferDesc{};
        constantBufferDesc.ByteWidth = byteWidth;
        constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        constantBufferDesc.Usage = D3D11_USAGE_DEFAULT;

        HR_CHECK(GraphicsDevice::Get().GetDevice()->CreateBuffer(&constantBufferDesc, nullptr, &m_buffer));
    }

    const Microsoft::WRL::ComPtr<ID3D11Buffer>& ConstantBuffer::GetBuffer() const
    {
        return m_buffer;
    }

    ID3D11Buffer* ConstantBuffer::GetRawBuffer() const
    {
        return m_buffer.Get();
    }
}