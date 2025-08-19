#include "UITitleBar.h"

static Core::Window* s_window;

//Get references to the icon textures
static Core::sPtr<Core::Texture> s_appIcon;
static Core::sPtr<Core::Texture> s_closeIcon;
static Core::sPtr<Core::Texture> s_maximizedIcon;
static Core::sPtr<Core::Texture> s_demaximizedIcon;
static Core::sPtr<Core::Texture> s_minimizedIcon;

void UITitleBar::Init() {
	s_window = &Core::App::GetWindow();

	//Set the references
	s_appIcon = Core::AssetManager::GetTexture("appIcon.png");
	s_closeIcon = Core::AssetManager::GetTexture("closeIcon.png");
	s_maximizedIcon = Core::AssetManager::GetTexture("maximizeIcon.png");
	s_demaximizedIcon = Core::AssetManager::GetTexture("deMaximizeIcon.png");
	s_minimizedIcon = Core::AssetManager::GetTexture("minimizeIcon.png");

	s_window->SetTitleBarSize(32, (s_closeIcon->width * 3) - 1);
}

void UITitleBar::Update() {
	// Create Title bar window
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 0,0 });
	ImGui::SetNextWindowPos({-1,-1});
	ImGui::SetNextWindowSize({ (float)s_window->GetWidth() + 2.0f, (float)s_window->GetTitleBarHeight() + 1.0f });
	ImGui::Begin("TitleBar", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar);
	
	// Draw App Icon
	ImGui::SetCursorPos({ 0, 0 });
	ImGui::Image((ImTextureID)s_appIcon->id, {(float)s_appIcon->width, (float)s_appIcon->height});

	ImGui::SameLine();

	// Draw App Title
	ImGui::SetCursorPosY(ImGui::GetStyle().WindowPadding.y);
	ImGui::PushFont(Core::ImGuiManager::GetFont("DefaultBold"));
	ImGui::Text("SFileExplorer");
	ImGui::PopFont();
	
	//Draw control buttons
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, { 0,0 });
	ImGui::SetCursorPos(ImVec2(s_window->GetWidth() - s_window->GetTitleBarControlWidth(), 0));

	// Minimize
	if (ImGui::ImageButton("Minimize", (ImTextureID)s_minimizedIcon->id, {(float)s_minimizedIcon->width, (float)s_minimizedIcon->height})) {
		s_window->Minimize();
	}	
	ImGui::SameLine();
	
	// Maximize
	auto maximizeIcon = s_window->GetMaximized() ? s_demaximizedIcon : s_maximizedIcon;
	if (ImGui::ImageButton("Maximize", (ImTextureID)maximizeIcon->id, {(float)maximizeIcon->width, (float)maximizeIcon->height})) {
		s_window->Maximize();
	}
	ImGui::SameLine();

	// Close
	if (ImGui::ImageButton("Close", (ImTextureID)s_closeIcon->id, {(float)s_closeIcon->width, (float)s_closeIcon->height})) {
		s_window->Close();
	}


	ImGui::PopStyleVar();

	ImGui::End();
	ImGui::PopStyleVar();
}

void UITitleBar::Shutdown() {

}
