#pragma once

#include "Core/Graphics/Resource/Resource.h"

namespace engine
{
    class IndexBuffer :
        public Resource
    {
    private:
        Microsoft::WRL::ComPtr<ID3D11Buffer> m_buffer;

    public:
        void Create(const std::vector<DWORD>& indices);

    public:
        const Microsoft::WRL::ComPtr<ID3D11Buffer>& GetBuffer() const;
        ID3D11Buffer* GetRawBuffer() const;
    };
}