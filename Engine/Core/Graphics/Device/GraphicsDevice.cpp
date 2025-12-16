#include "pch.h"
#include "GraphicsDevice.h"

#include "Core/Graphics/Geometry/GeometryGenerator.h"

#include <dxgi1_5.h>
#include <dxgi1_6.h>
#include <d3dcompiler.h>

#include "Common/Utility/Profiling.h"

namespace engine
{
    void GraphicsDevice::Initialize(
        HWND hWnd,
        UINT resolutionWidth,
        UINT resolutionHeight,
        UINT screenWidth,
        UINT screenHeight,
        bool isFullScreen,
        bool useVsync)
    {
        m_hWnd = hWnd;
        m_resolutionWidth = resolutionWidth;
        m_resolutionHeight = resolutionHeight;
        m_screenWidth = screenWidth;
        m_screenHeight = screenHeight;
        m_isFullScreen = isFullScreen;
        SetVsync(useVsync);

        // create device, device context, swap chain
        {
            UINT dxgiFactoryCreationFlags = 0;
#ifdef _DEBUG
            dxgiFactoryCreationFlags |= DXGI_CREATE_FACTORY_DEBUG;
#endif // _DEBUG

            Microsoft::WRL::ComPtr<IDXGIFactory5> dxgiFactory;
            HR_CHECK(CreateDXGIFactory2(dxgiFactoryCreationFlags, IID_PPV_ARGS(&dxgiFactory)));

            // create device, device context
            {
                Microsoft::WRL::ComPtr<IDXGIAdapter1> adapter = GetHighPerformanceAdapter(dxgiFactory);

                UINT d3dCreationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#ifdef _DEBUG
                d3dCreationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif // _DBUG

                const D3D_FEATURE_LEVEL featureLevels[]{
                    D3D_FEATURE_LEVEL_12_2,
                    D3D_FEATURE_LEVEL_12_1,
                    D3D_FEATURE_LEVEL_12_0,
                    D3D_FEATURE_LEVEL_11_1,
                    D3D_FEATURE_LEVEL_11_0
                };

                D3D_FEATURE_LEVEL actualFeatureLevel;

                HR_CHECK(D3D11CreateDevice(
                    adapter.Get(),
                    D3D_DRIVER_TYPE_UNKNOWN,
                    nullptr,
                    d3dCreationFlags,
                    featureLevels,
                    ARRAYSIZE(featureLevels),
                    D3D11_SDK_VERSION,
                    &m_device,
                    &actualFeatureLevel,
                    &m_deviceContext));
            }

            // create swap chain
            {
                BOOL allowTearing = FALSE;
                HR_CHECK(dxgiFactory->CheckFeatureSupport(DXGI_FEATURE_PRESENT_ALLOW_TEARING, &allowTearing, sizeof(allowTearing)));

                if (allowTearing)
                {
                    m_tearingSupport = true;
                }

                DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};
                swapChainDesc.Width = m_isFullScreen ? m_screenWidth : m_resolutionWidth;
                swapChainDesc.Height = m_isFullScreen ? m_screenHeight : m_resolutionHeight;
                swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
                swapChainDesc.SampleDesc.Count = 1;
                swapChainDesc.SampleDesc.Quality = 0;
                swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
                swapChainDesc.BufferCount = 2;
                swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
                swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_IGNORE;
                swapChainDesc.Scaling = DXGI_SCALING_NONE;
                swapChainDesc.Stereo = FALSE;
                swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
                if (m_tearingSupport)
                {
                    swapChainDesc.Flags |= DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING;
                }

                HR_CHECK(dxgiFactory->CreateSwapChainForHwnd(
                    m_device.Get(),
                    m_hWnd,
                    &swapChainDesc,
                    nullptr,
                    nullptr,
                    &m_swapChain));
            }
        }

        // directx alt+enter 전체화면 전환 해제
        if (Microsoft::WRL::ComPtr<IDXGIFactory1> factory;
            SUCCEEDED(m_swapChain->GetParent(__uuidof (IDXGIFactory1), &factory)))
        {
            factory->MakeWindowAssociation(m_hWnd, DXGI_MWA_NO_ALT_ENTER);
        }

        CreateSizeDependentResources();

        Microsoft::WRL::ComPtr<IDXGIAdapter> dxgiAdapter;
        Microsoft::WRL::ComPtr<IDXGIDevice3> dxgiDevice;
        m_device.As(&dxgiDevice);
        dxgiDevice->GetAdapter(dxgiAdapter.GetAddressOf());
        dxgiAdapter.As(&m_dxgiAdapter);

        // Create Fullscreen Quad Mesh
        {
            MeshData quadData = GeometryGenerator::CreateFullscreenQuad();
            m_fullscreenQuadMesh = std::make_unique<Mesh>();
            m_fullscreenQuadMesh->Initialize(m_device, quadData);
        }

        // Create Default Shaders & Layout
        {
            Microsoft::WRL::ComPtr<ID3DBlob> vsBlob;
            CompileShaderFromFile("Shader/Vertex/DefaultVertexShader.hlsl", "main", "vs_5_0", vsBlob);
            HR_CHECK(m_device->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), nullptr, &m_defaultVS));

            D3D11_INPUT_ELEMENT_DESC layout[] =
            {
                { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
                { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
                { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            };
            HR_CHECK(m_device->CreateInputLayout(layout, ARRAYSIZE(layout), vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), &m_defaultInputLayout));

            Microsoft::WRL::ComPtr<ID3DBlob> psBlob;
            CompileShaderFromFile("Shader/Pixel/DefaultPixelShader.hlsl", "main", "ps_5_0", psBlob);
            HR_CHECK(m_device->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), nullptr, &m_defaultPS));
        }

        // Create Sampler State
        {
            D3D11_SAMPLER_DESC samplerDesc{};
            samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
            samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
            samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
            samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
            samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
            samplerDesc.MinLOD = 0;
            samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

            HR_CHECK(m_device->CreateSamplerState(&samplerDesc, &m_defaultSamplerState));
        }
    }

    bool GraphicsDevice::Resize(
        UINT resolutionWidth,
        UINT resolutionHeight,
        UINT screenWidth,
        UINT screenHeight,
        bool isFullScreen)
    {
        if (m_resolutionWidth == resolutionWidth &&
            m_resolutionHeight == resolutionHeight &&
            m_isFullScreen == isFullScreen)
        {
            return false;
        }

        if (resolutionWidth == 0 || resolutionHeight == 0)
        {
            return false;
        }

        m_resolutionWidth = resolutionWidth;
        m_resolutionHeight = resolutionHeight;
        m_isFullScreen = isFullScreen;

        m_deviceContext->OMSetRenderTargets(0, nullptr, nullptr);
        m_backBufferRTV.Reset();
        m_gameRTV.Reset();
        m_gameDSV.Reset();
        m_gameSRV.Reset();

        UINT swapChainFlags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
        if (m_tearingSupport)
        {
            swapChainFlags |= DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING;
        }
        HR_CHECK(m_swapChain->ResizeBuffers(
            0,
            m_isFullScreen ? m_screenWidth : m_resolutionWidth,
            m_isFullScreen ? m_screenHeight : m_resolutionHeight,
            DXGI_FORMAT_UNKNOWN,
            swapChainFlags));

        CreateSizeDependentResources();

        return true;
    }

    void GraphicsDevice::BeginDraw(const Color& clearColor)
    {
        m_deviceContext->OMSetRenderTargets(1, m_gameRTV.GetAddressOf(), m_gameDSV.Get());

        m_deviceContext->ClearRenderTargetView(m_gameRTV.Get(), clearColor);
        m_deviceContext->ClearDepthStencilView(m_gameDSV.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

        m_deviceContext->RSSetViewports(1, &m_gameViewport);
    }

    void GraphicsDevice::BackBufferDraw()
    {
        m_deviceContext->OMSetRenderTargets(1, m_backBufferRTV.GetAddressOf(), nullptr);

        m_deviceContext->ClearRenderTargetView(m_backBufferRTV.Get(), Color(0.0f, 0.0f, 0.0f, 1.0f));

        m_deviceContext->RSSetViewports(1, &m_backBufferViewport);

        // Blit
        {
            // Input Assembler
            m_deviceContext->IASetInputLayout(m_defaultInputLayout.Get());
            m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

            // Shaders
            m_deviceContext->VSSetShader(m_defaultVS.Get(), nullptr, 0);
            m_deviceContext->PSSetShader(m_defaultPS.Get(), nullptr, 0);

            // Resources
            m_deviceContext->PSSetShaderResources(0, 1, m_gameSRV.GetAddressOf());
            m_deviceContext->PSSetSamplers(0, 1, m_defaultSamplerState.GetAddressOf());

            // Draw
            m_fullscreenQuadMesh->Render(m_deviceContext);

            // Unbind
            ID3D11ShaderResourceView* nullSRV = nullptr;
            m_deviceContext->PSSetShaderResources(0, 1, &nullSRV);
        }
    }

    void GraphicsDevice::EndDraw()
    {
        m_swapChain->Present(m_syncInterval, m_presentFlags);

        // vram usage
        {
            DXGI_QUERY_VIDEO_MEMORY_INFO memInfo{};
            m_dxgiAdapter->QueryVideoMemoryInfo(0, DXGI_MEMORY_SEGMENT_GROUP_LOCAL, &memInfo);
            Profiling::UpdateVRAMUsage(memInfo.CurrentUsage);
        }
    }

    const Microsoft::WRL::ComPtr<ID3D11Device>& GraphicsDevice::GetDevice() const
    {
        return m_device;
    }

    const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& GraphicsDevice::GetDeviceContext() const
    {
        return m_deviceContext;
    }

    const Microsoft::WRL::ComPtr<IDXGISwapChain1>& GraphicsDevice::GetSwapChain() const
    {
        return m_swapChain;
    }

    const Microsoft::WRL::ComPtr<ID3D11RenderTargetView>& GraphicsDevice::GetRenderTargetView() const
    {
        return m_gameRTV;
    }

    const Microsoft::WRL::ComPtr<ID3D11DepthStencilView>& GraphicsDevice::GetDepthStencilView() const
    {
        return m_gameDSV;
    }

    const D3D11_VIEWPORT& GraphicsDevice::GetViewport() const
    {
        return m_gameViewport;
    }

    void GraphicsDevice::SetVsync(bool useVsync)
    {
        m_useVsync = useVsync;
        if (m_useVsync)
        {
            m_syncInterval = 1;
            m_presentFlags = 0;
        }
        else
        {
            m_syncInterval = 0;
            m_presentFlags = DXGI_PRESENT_ALLOW_TEARING;
        }
    }

    void GraphicsDevice::CompileShaderFromFile(
        const std::filesystem::path& fileName,
        const std::string& entryPoint,
        const std::string& shaderModel,
        Microsoft::WRL::ComPtr<ID3DBlob>& blobOut)
    {
        DWORD shaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef _DEBUG
        shaderFlags |= D3DCOMPILE_DEBUG;
        shaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif // _DEBUG

        Microsoft::WRL::ComPtr<ID3DBlob> errorBlob;

        HR_CHECK(D3DCompileFromFile(
            (fileName).c_str(),
            nullptr,
            D3D_COMPILE_STANDARD_FILE_INCLUDE,
            entryPoint.c_str(),
            shaderModel.c_str(),
            shaderFlags,
            0,
            &blobOut,
            &errorBlob));
    }

    void GraphicsDevice::CreateSizeDependentResources()
    {
        Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;
        HR_CHECK(m_swapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer)));
        HR_CHECK(m_device->CreateRenderTargetView(backBuffer.Get(), nullptr, &m_backBufferRTV));

        D3D11_TEXTURE2D_DESC depthStencilDesc{};
        depthStencilDesc.Width = m_resolutionWidth;
        depthStencilDesc.Height = m_resolutionHeight;
        depthStencilDesc.MipLevels = 1;
        depthStencilDesc.ArraySize = 1;
        depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
        depthStencilDesc.SampleDesc.Count = 1;
        depthStencilDesc.SampleDesc.Quality = 0;
        depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
        depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
        depthStencilDesc.CPUAccessFlags = 0;
        depthStencilDesc.MiscFlags = 0;

        Microsoft::WRL::ComPtr<ID3D11Texture2D> depthStencilTexture;
        HR_CHECK(m_device->CreateTexture2D(&depthStencilDesc, nullptr, &depthStencilTexture));

        D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc{};
        depthStencilViewDesc.Format = depthStencilDesc.Format;
        depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
        depthStencilViewDesc.Texture2D.MipSlice = 0;

        HR_CHECK(m_device->CreateDepthStencilView(depthStencilTexture.Get(), &depthStencilViewDesc, &m_gameDSV));

        // 1. Create Off-screen Texture
        D3D11_TEXTURE2D_DESC gameTextureDesc{};
        gameTextureDesc.Width = m_resolutionWidth;
        gameTextureDesc.Height = m_resolutionHeight;
        gameTextureDesc.MipLevels = 1;
        gameTextureDesc.ArraySize = 1;
        gameTextureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        gameTextureDesc.SampleDesc.Count = 1;
        gameTextureDesc.Usage = D3D11_USAGE_DEFAULT;
        gameTextureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
        gameTextureDesc.CPUAccessFlags = 0;
        gameTextureDesc.MiscFlags = 0;

        Microsoft::WRL::ComPtr<ID3D11Texture2D> gameTexture;
        HR_CHECK(m_device->CreateTexture2D(&gameTextureDesc, nullptr, &gameTexture));

        // 2. Create RTV for Game
        HR_CHECK(m_device->CreateRenderTargetView(gameTexture.Get(), nullptr, &m_gameRTV));

        // 3. Create SRV for Game (to be sampled in EndDraw)
        HR_CHECK(m_device->CreateShaderResourceView(gameTexture.Get(), nullptr, &m_gameSRV));

        m_gameViewport.TopLeftX = 0.0f;
        m_gameViewport.TopLeftY = 0.0f;
        m_gameViewport.Width = static_cast<float>(m_resolutionWidth);
        m_gameViewport.Height = static_cast<float>(m_resolutionHeight);
        m_gameViewport.MinDepth = 0.0f;
        m_gameViewport.MaxDepth = 1.0f;

        m_backBufferViewport.TopLeftX = 0.0f;
        m_backBufferViewport.TopLeftY = 0.0f;
        m_backBufferViewport.MinDepth = 0.0f;
        m_backBufferViewport.MaxDepth = 1.0f;

        float targetAspectRatio = static_cast<float>(m_resolutionWidth) / static_cast<float>(m_resolutionHeight);
        float screenAspectRatio = static_cast<float>(m_screenWidth) / static_cast<float>(m_screenHeight);

        if (m_isFullScreen)
        {
            float viewWidth = static_cast<float>(m_screenWidth);
            float viewHeight = static_cast<float>(m_screenHeight);
            float viewX = 0.0f;
            float viewY = 0.0f;

            if (screenAspectRatio > targetAspectRatio)
            {
                viewWidth = m_screenHeight * targetAspectRatio;
                viewX = (m_screenWidth - viewWidth) * 0.5f;
            }
            else
            {
                viewHeight = m_screenWidth / targetAspectRatio;
                viewY = (m_screenHeight - viewHeight) * 0.5f;
            }

            m_backBufferViewport.TopLeftX = viewX;
            m_backBufferViewport.TopLeftY = viewY;
            m_backBufferViewport.Width = viewWidth;
            m_backBufferViewport.Height = viewHeight;
        }
        else
        {
            m_backBufferViewport.Width = static_cast<float>(m_resolutionWidth);
            m_backBufferViewport.Height = static_cast<float>(m_resolutionHeight);
        }
    }

    Microsoft::WRL::ComPtr<IDXGIAdapter1> GraphicsDevice::GetHighPerformanceAdapter(Microsoft::WRL::ComPtr<IDXGIFactory5> factory)
    {
        Microsoft::WRL::ComPtr<IDXGIFactory6> factory6;
        HRESULT hr = factory.As(&factory6);

        Microsoft::WRL::ComPtr<IDXGIAdapter1> adapter;

        if (SUCCEEDED(hr))
        {
            for (UINT i = 0;
                factory6->EnumAdapterByGpuPreference(
                    i,
                    DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, // 핵심: 고성능 GPU 요청
                    IID_PPV_ARGS(&adapter)) != DXGI_ERROR_NOT_FOUND;
                    ++i)
            {
                DXGI_ADAPTER_DESC1 desc;
                adapter->GetDesc1(&desc);

                if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
                {
                    continue;
                }

                if (SUCCEEDED(D3D11CreateDevice(
                    adapter.Get(),
                    D3D_DRIVER_TYPE_UNKNOWN,
                    nullptr,
                    0,
                    nullptr,
                    0,
                    D3D11_SDK_VERSION,
                    nullptr, nullptr, nullptr)))
                {
                    break;
                }
            }
        }

        if (adapter == nullptr)
        {
            for (UINT i = 0; factory->EnumAdapters1(i, &adapter) != DXGI_ERROR_NOT_FOUND; ++i)
            {
                DXGI_ADAPTER_DESC1 desc;
                adapter->GetDesc1(&desc);

                if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
                {
                    continue;
                }

                if (SUCCEEDED(D3D11CreateDevice(
                    adapter.Get(),
                    D3D_DRIVER_TYPE_UNKNOWN,
                    nullptr,
                    0,
                    nullptr,
                    0,
                    D3D11_SDK_VERSION,
                    nullptr, nullptr, nullptr)))
                {
                    break;
                }
            }
        }

        return adapter;
    }
}
