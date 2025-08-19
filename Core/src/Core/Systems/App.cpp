#include "App.h"

static Core::App* s_instance = nullptr;
Core::App& Core::App::Get() { return *s_instance; }

void Core::App::Init() {
	if (!s_instance) s_instance = this;

	m_window = MakeWindow(m_appName, 1280, 720);

	m_imManager = SetUPtr<ImGuiManager>();

	AssetManager::Init();

	UIManager::Init();

	AppStart();
}

void Core::App::Run() {
	while (IsRunning()) {
		AppUpdate();

		ImGuiManager::DrawBegin(m_clearColor);

		UIManager::Update();

		AssetManager::Dispatch();

		ImGuiManager::DrawEnd();

		m_window->Update();
	}
}

void Core::App::Shutdown() {
	AppEnd();

	UIManager::Shutdown();

	AssetManager::Shutdown();
}

bool Core::App::IsRunning() {
	if (m_window->ShouldClose()) return false;
	return true;
}
