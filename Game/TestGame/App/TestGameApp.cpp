#include "pch.h"
#include "TestGameApp.h"

#include "Framework/Scene/SceneManager.h"
#include "Framework/Scene/Scene.h"

#include "Script/TestScript.h"

namespace game
{
	engine::WindowSettings g_default{
		1920,
		1080,
		{ "1280x720", "1920x1080", "2560x1440", "3840x2160" },
		false,
		false
	};

	TestGameApp::TestGameApp()
		: engine::WinApp()
	{
	}

	TestGameApp::TestGameApp(const std::filesystem::path& settingFilePath)
		: engine::WinApp(settingFilePath, g_default)
	{
	}

	void TestGameApp::Initialize()
	{
		engine::WinApp::Initialize();

		engine::SceneManager::Get().CreateScene("SampleScene1");

		engine::SceneManager::Get().ChangeScene("SampleScene1");
	}

	void TestGameApp::OnUpdate()
	{
		static bool once = true;
		if (once)
		{
			once = false;
			engine::Scene* scene = engine::SceneManager::Get().GetCurrentScene();

			engine::Ptr<engine::GameObject> gameObject = scene->CreateGameObject("TestGameObject");
			engine::Ptr<TestScript> testScript = gameObject->AddComponent<TestScript>();
		}
	}
}