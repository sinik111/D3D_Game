#pragma once

#include "Core/Graphics/Resource/Resource.h"

namespace engine
{
    class VertexBuffer :
        public Resource
    {
    private:
        Microsoft::WRL::ComPtr<ID3D11Buffer> m_buffer;
        UINT m_bufferStride = 0;

    public:
        template <typename T>
        void Create(const Microsoft::WRL::ComPtr<ID3D11Device>& device, const std::vector<T>& vertices)
        {
            m_bufferStride = sizeof(T);

            D3D11_BUFFER_DESC desc{};
            desc.ByteWidth = static_cast<UINT>(sizeof(T) * vertices.size());
            desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
            desc.Usage = D3D11_USAGE_DEFAULT;
            desc.CPUAccessFlags = 0;

            D3D11_SUBRESOURCE_DATA data{};
            data.pSysMem = vertices.data();

            HR_CHECK(device->CreateBuffer(&desc, &data, &m_buffer));
        }

    public:
        const Microsoft::WRL::ComPtr<ID3D11Buffer>& GetBuffer() const;
        ID3D11Buffer* GetRawBuffer() const;
        UINT GetBufferStride() const;
    };
}