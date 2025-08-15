#include "App.h"

static Core::App* s_instance = nullptr;
Core::App& Core::App::Get() { return *s_instance; }

void Core::App::Init() {
	if (!s_instance) s_instance = this;

	m_window = MakeWindow(m_appName, 1280, 720);

	m_imManager = SetUPtr<ImGuiManager>();

	UIManager::Init();

	AppStart();
}

void Core::App::Run() {
	while (IsRunning()) {
		ImGuiManager::DrawBegin(m_clearColor);

		AppUpdate();
		UIManager::Update();

		ImGuiManager::DrawEnd();

		m_window->Update();
	}
}

void Core::App::Shutdown() {
	AppEnd();

	UIManager::Shutdown();
}

bool Core::App::IsRunning() {
	if (m_window->ShouldClose()) return false;
	return true;
}
