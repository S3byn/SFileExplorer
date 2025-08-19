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
		float currentFrame = (float)System::GetTime();
		m_deltaTime = currentFrame - m_lastFrame;
		m_lastFrame = currentFrame;

		AppUpdate(m_deltaTime);

		ImGuiManager::DrawBegin(m_clearColor);

		UIManager::Update(m_deltaTime);

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
