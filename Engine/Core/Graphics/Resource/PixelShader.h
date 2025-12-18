#pragma once

#include "Core/Graphics/Resource/Resource.h"

namespace engine
{
    class PixelShader :
        public Resource
    {
    private:
        Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;

    public:
        void Create(const std::string& filePath);

    public:
        const Microsoft::WRL::ComPtr<ID3D11PixelShader>& GetShader() const;
        ID3D11PixelShader* GetRawShader() const;
    };
}