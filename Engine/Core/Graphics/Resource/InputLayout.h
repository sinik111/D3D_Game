#pragma once

#include "Core/Graphics/Resource/Resource.h"

namespace engine
{
    class InputLayout :
        public Resource
    {
    private:
        Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;

    public:
        void Create(
            const Microsoft::WRL::ComPtr<ID3D10Blob>& blob,
            const D3D11_INPUT_ELEMENT_DESC* desc,
            UINT numElements);

    public:
        const Microsoft::WRL::ComPtr<ID3D11InputLayout>& GetInputLayout() const;
        ID3D11InputLayout* GetRawInputLayout() const;
    };
}