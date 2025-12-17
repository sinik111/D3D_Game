#pragma once

#include "Core/Graphics/Resource/Resource.h"

namespace engine
{
    class VertexShader :
        public Resource
    {
    private:
        Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;

    public:
        void Create(const Microsoft::WRL::ComPtr<ID3D11Device>& device, const std::string& filePath);

    public:
        const Microsoft::WRL::ComPtr<ID3D11VertexShader>& GetShader() const;
        ID3D11VertexShader* GetRawShader() const;
    };
}