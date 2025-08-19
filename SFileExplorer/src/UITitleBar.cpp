#include "UITitleBar.h"

static Core::Window* s_window;

void UITitleBar::Init() {
	s_window = &Core::App::GetWindow();
	s_window->SetTitleBarSize(32, 128);
}

void UITitleBar::Update() {
	ImGui::SetNextWindowPos({0,0});
	ImGui::SetNextWindowSize({ (float)s_window->GetWidth(), (float)s_window->GetTitleBarHeight() });
	ImGui::Begin("TitleBar", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar);
	
	ImGui::PushFont(Core::ImGuiManager::GetFont("DefaultBold"));
	ImGui::Text("SFileExplorer");
	ImGui::PopFont();

	ImGui::SetCursorPos(ImVec2(s_window->GetWidth() - s_window->GetTitleBarControlWidth(), 0));
	if (ImGui::Button("X")) {
		s_window->Close();
	}
	ImGui::SameLine();

	if (ImGui::Button("O")) {
		s_window->Maximize();
	}
	ImGui::SameLine();

	if (ImGui::Button("-")) {
		s_window->Minimize();
	}

	ImGui::End();
}

void UITitleBar::Shutdown() {
}
