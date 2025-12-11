#pragma once

#include "Core/Graphics/Device/GraphicsDevice.h"

namespace engine
{
	struct WindowSettings
	{
		int resolutionWidth = 1280;
		int resolutionHeight = 720;
		std::vector<std::string> supportedResolutions{ "1280x720", "1920x1080", "2560x1440", "3840x2160" };
		bool isFullScreen = false;
		bool useVsync = true;
	};

	class WinApp
	{
	protected:
		HWND m_hWnd = nullptr;
		HINSTANCE m_hInstance = nullptr;
		HICON m_hIcon = nullptr;
		HCURSOR m_hCursor = nullptr;
		HICON m_hIconSmall = nullptr;

		std::string m_className = "game123";
		std::string m_windowName = "Default";
		std::string m_settingFilePath;
		WindowSettings m_settings;

		UINT m_classStyle = 0;
		DWORD m_windowStyle = 0;
		int m_x = 0;
		int m_y = 0;
		int m_screenWidth = 0;
		int m_screenHeight = 0;

		GraphicsDevice m_graphicsDevice;

	protected:
		WinApp(const std::string& settingFilePath = "config.json", const WindowSettings& defaultSetting = {});
		virtual ~WinApp();

	public:
		virtual void Initialize();
		virtual void Shutdown();
		void Run();

	private:
		void Update();
		void Render();

		void ValidateSettings();
		void SetWindowMode(bool isFullScreen);
		void SetResolution(int width, int height, bool isFullScreen);

	protected:
		virtual LRESULT MessageProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
		friend LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
	};
}
