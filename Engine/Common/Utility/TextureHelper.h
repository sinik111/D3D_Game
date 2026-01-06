#pragma once

#include <d3d11.h>

namespace engine
{
    inline D3D11_TEXTURE2D_DESC GetTextureDescFromSRV(ID3D11ShaderResourceView* srv)
    {
        D3D11_TEXTURE2D_DESC desc{};

        if (srv == nullptr)
        {
            return desc;
        }

        ID3D11Resource* resource = nullptr;
        srv->GetResource(&resource);

        if (resource)
        {
            ID3D11Texture2D* texture = nullptr;
            HRESULT hr = resource->QueryInterface(__uuidof(ID3D11Texture2D), (void**)&texture);

            if (SUCCEEDED(hr) && texture)
            {
                texture->GetDesc(&desc);
                
                texture->Release();
            }

            resource->Release();
        }

        return desc;
    }
}