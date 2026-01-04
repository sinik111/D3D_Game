#include "pch.h"
#include "InputLayout.h"

#include "Core/Graphics/Device/GraphicsDevice.h"

namespace engine
{
    void InputLayout::Create(
        const Microsoft::WRL::ComPtr<ID3D10Blob>& blob,
        const D3D11_INPUT_ELEMENT_DESC* desc,
        UINT numElements)
    {
        GraphicsDevice::Get().GetDevice()->CreateInputLayout(
            desc,
            numElements,
            blob->GetBufferPointer(),
            blob->GetBufferSize(),
            &m_inputLayout);
    }

    const Microsoft::WRL::ComPtr<ID3D11InputLayout>& InputLayout::GetInputLayout() const
    {
        return m_inputLayout;
    }

    ID3D11InputLayout* InputLayout::GetRawInputLayout() const
    {
        return m_inputLayout.Get();
    }
}