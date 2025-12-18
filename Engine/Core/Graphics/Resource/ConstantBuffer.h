#pragma once

#include "Core/Graphics/Resource/Resource.h"

namespace engine
{
    class ConstantBuffer :
        public Resource
    {
    private:
        Microsoft::WRL::ComPtr<ID3D11Buffer> m_buffer;

    public:
        void Create(UINT byteWidth);

    public:
        const Microsoft::WRL::ComPtr<ID3D11Buffer>& GetBuffer() const;
        ID3D11Buffer* GetRawBuffer() const;
    };
}