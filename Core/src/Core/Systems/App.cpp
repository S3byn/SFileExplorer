#include "App.h"

static Core::App* s_instance = nullptr;
Core::App& Core::App::Get() { return *s_instance; }

void Core::App::Init() {
	if (!s_instance) s_instance = this;

	m_window = MakeWindow(m_appName, 1280, 720);

	m_imManager = std::make_unique<ImGuiManager>();
}

void Core::App::Run() {
	while (IsRunning()) {
		ImGuiManager::DrawBegin(m_clearColor);

		AppUpdate();

		ImGuiManager::DrawEnd();

		m_window->Update();
	}
}

void Core::App::Shutdown() {

}

bool Core::App::IsRunning() {
	if (m_window->ShouldClose()) return false;
	return true;
}
