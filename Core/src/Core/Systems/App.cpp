#include "App.h"

void Core::App::Init() {
	m_window = CreateWindow("Hello World", 1280, 720);
}

void Core::App::Run() {
	while (IsRunning()) {
		m_window->Update();
	}
}

void Core::App::Shutdown() {

}

bool Core::App::IsRunning() {
	if (m_window->ShouldClose()) return false;
	return true;
}
