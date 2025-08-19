#include "UIManager.h"

static Core::UIManager* s_instance = nullptr;
Core::UIManager& Core::UIManager::Get() { return *s_instance; }

void Core::UIManager::Init() {
	if (!s_instance) s_instance = new UIManager;
}

void Core::UIManager::Update(float delta) {
	for (const auto& component : Get().m_components) {
		component.second->Update(delta);
	}
}

void Core::UIManager::Shutdown() {
	for (const auto& component : Get().m_components) {
		component.second->Shutdown();
	}

	if (s_instance) delete s_instance;
}

