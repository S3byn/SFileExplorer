#include "UIMainView.h"
#include "UIMenuBar.h"

#include <iostream>

static Core::Window* s_window;
static Core::sPtr<UIMenuBar> s_uiMenuBar;

static float s_mainViewOffset = 0.0f;

//Icons
static Core::sPtr<Core::Texture> s_directoryIcon;
static Core::sPtr<Core::Texture> s_fileIcon;


void UIMainView::Init() {
	s_window = &Core::App::GetWindow();
	s_uiMenuBar = Core::UIManager::GetComponent<UIMenuBar>();
	s_mainViewOffset = (float)s_window->GetTitleBarHeight() + s_uiMenuBar->menuBarHeight - 2.0f;

	files = Core::SetSPtr<Explorer::Directory>(currentDirectory);

	s_directoryIcon = Core::AssetManager::GetTexture("directoryIcon.png");
	s_fileIcon = Core::AssetManager::GetTexture("fileIcon.png");
}

void UIMainView::Update(float delta) {
	// Set Window BG
	ImGui::PushStyleColor(ImGuiCol_WindowBg, Core::ImGuiXtra::HexColor(0x181133, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_Border, Core::ImGuiXtra::HexColor(0x403363, 1.0f));

	ImGui::PushStyleColor(ImGuiCol_Button, { 0.0f, 0.0f, 0.0f, 0.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, Core::ImGuiXtra::HexColor(0xBA9EFF, 0.2f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, Core::ImGuiXtra::HexColor(0xBA9EFF, 0.4f));

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 2.0f, 2.0f });
	ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 6.0f);

	// Create window
	ImGui::SetNextWindowPos({-1, s_mainViewOffset});
	ImGui::SetNextWindowSize({(float)s_window->GetWidth() + 2, (float)s_window->GetHeight() - s_mainViewOffset + 1.0f});
	ImGui::Begin(UIMainView::GetName().c_str(), nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar);
	
	int i = 0;
	for (const auto& file : *files) {
		if(mainMenuIndex == i){
			ImGui::PushStyleColor(ImGuiCol_Button, Core::ImGuiXtra::HexColor(0xBA9EFF, 0.2f));
		}
		else {
			ImGui::PushStyleColor(ImGuiCol_Button, { 0.0f, 0.0f, 0.0f, 0.0f });
		}

		Core::ImGuiXtra::Button(("##" + file.name).c_str(), { s_window->GetWidth() - ImGui::GetStyle().WindowPadding.x, s_directoryIcon->height }, delta);
		
		if (ImGui::IsItemHovered() && (ImGui::IsMouseReleased(ImGuiMouseButton_Left) || ImGui::IsMouseReleased(ImGuiMouseButton_Right))) {
			mainMenuIndex = i;
		}
		if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
			
		}
		if (!ImGui::IsAnyItemHovered() && (ImGui::IsMouseReleased(ImGuiMouseButton_Left) || ImGui::IsMouseReleased(ImGuiMouseButton_Right))) {
			mainMenuIndex = -1;
		}

		ImGui::PopStyleColor();
		ImGui::SameLine();

		ImGui::SetCursorPosX(ImGui::GetCursorPosX() - s_window->GetWidth());
		Core::ImGuiXtra::Image(file.isDirectory ? s_directoryIcon : s_fileIcon);
		ImGui::SameLine();
		ImGui::Text(file.name.c_str());
		i++;
	}
	
	ImGui::End();

	ImGui::PopStyleColor(5);
	ImGui::PopStyleVar(2);
}

void UIMainView::Shutdown() {

}
