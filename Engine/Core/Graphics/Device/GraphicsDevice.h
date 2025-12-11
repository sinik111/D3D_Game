#pragma once

#include "Core/Graphics/Resource/Mesh.h"

namespace engine
{
	class GraphicsDevice
	{
	private:
		Microsoft::WRL::ComPtr<ID3D11Device> m_device;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_deviceContext;
		Microsoft::WRL::ComPtr<IDXGISwapChain1> m_swapChain;

		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_gameRTV;
		// 나중에 여러장을 gameRTV에 그리게 되면 DSV는 필요없어 질 수도..
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_gameDSV;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_gameSRV;

		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_backBufferRTV;

		Microsoft::WRL::ComPtr<IDXGIAdapter3> m_dxgiAdapter;

		// Resources for Blit
		std::unique_ptr<Mesh> m_fullscreenQuadMesh;
		Microsoft::WRL::ComPtr<ID3D11VertexShader> m_defaultVS;
		Microsoft::WRL::ComPtr<ID3D11PixelShader> m_defaultPS;
		Microsoft::WRL::ComPtr<ID3D11InputLayout> m_defaultInputLayout;
		Microsoft::WRL::ComPtr<ID3D11SamplerState> m_defaultSamplerState;

		HWND m_hWnd = nullptr;
		UINT m_resolutionWidth = 0;
		UINT m_resolutionHeight = 0;
		UINT m_screenWidth = 0;
		UINT m_screenHeight = 0;
		D3D11_VIEWPORT m_gameViewport{};
		D3D11_VIEWPORT m_backBufferViewport{};

		UINT m_syncInterval = 0;
		UINT m_presentFlags = 0;
		bool m_useVsync = false;
		bool m_tearingSupport = false;
		bool m_isFullScreen = false;

	public:
		void Initialize(
			HWND hWnd,
			UINT resolutionWidth,
			UINT resolutionHeight,
			UINT screenWidth,
			UINT screenHeight,
			bool isFullScreen,
			bool useVsync);
		bool Resize(UINT resolutionWidth,
			UINT resolutionHeight,
			UINT screenWidth,
			UINT screenHeight,
			bool isFullScreen);
		void BeginDraw(const Color& clearColor = {});
		void EndDraw();

		const Microsoft::WRL::ComPtr<ID3D11Device>& GetDevice() const;
		const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& GetDeviceContext() const;
		const Microsoft::WRL::ComPtr<IDXGISwapChain1>& GetSwapChain() const;
		const Microsoft::WRL::ComPtr<ID3D11RenderTargetView>& GetRenderTargetView() const;
		const Microsoft::WRL::ComPtr<ID3D11DepthStencilView>& GetDepthStencilView() const;
		const D3D11_VIEWPORT& GetViewport() const;

		void SetVsync(bool useVsync);

	public:
		static void CompileShaderFromFile(
			const std::string& fileName,
			const std::string& entryPoint,
			const std::string& shaderModel,
			Microsoft::WRL::ComPtr<ID3DBlob>& blobOut);

	private:
		void CreateSizeDependentResources();
	};
}
