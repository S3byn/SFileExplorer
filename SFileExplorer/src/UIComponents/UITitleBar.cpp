#include "UITitleBar.h"
#include "UIMenuBar.h"

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

	s_window->SetTitleBarSize(32, s_closeIcon->width * 3 - 1);
}

void UITitleBar::Update(float delta) {
	// Set Window BG
	ImGui::PushStyleColor(ImGuiCol_WindowBg, Core::ImGuiXtra::HexColor(0x0C0719, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_Border, Core::ImGuiXtra::HexColor(0x403363, 1.0f));

	// Set Buttons
	ImGui::PushStyleColor(ImGuiCol_Button, { 0.0f, 0.0f, 0.0f, 0.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 1.0f, 1.0f, 1.0f, 0.2f });
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, { 1.0f, 1.0f, 1.0f, 0.4f });
	
	// Create Title bar window
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 0.0f, 0.0f });
	ImGui::SetNextWindowPos({-1,-1});
	ImGui::SetNextWindowSize({ (float)s_window->GetWidth() + 2.0f, (float)s_window->GetTitleBarHeight() + 1.0f });
	ImGui::Begin(UITitleBar::GetName().c_str(), nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar);
	
	// Draw App Icon
	ImGui::SetCursorPos({ 0.0f, 0.0f });
	ImGui::Image((ImTextureID)s_appIcon->id, { (float)s_appIcon->width, (float)s_appIcon->height });
	ImGui::SameLine();

	// Draw App Title
	ImGui::SetCursorPosY(ImGui::GetStyle().WindowPadding.y);
	ImGui::PushFont(Core::ImGuiManager::GetFont("DefaultBold"));

	ImGui::Text("SFileExplorer");
	ImGui::PopFont();
	
	// Draw control buttons
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, { 1.0f, 1.0f });
	ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 6.0f);
	ImGui::SetCursorPos({ (float)s_window->GetWidth() - (float)s_window->GetTitleBarControlWidth(), 0.0f } );
	
	// Minimize
	if (Core::ImGuiXtra::ImageButton("Minimize", s_minimizedIcon, delta)) {
		s_window->Minimize();
	}
	ImGui::SameLine();
	
	// Maximize
	auto maximizeIcon = s_window->GetMaximized() ? s_demaximizedIcon : s_maximizedIcon;
	if (Core::ImGuiXtra::ImageButton("Maximized", maximizeIcon, delta)) {
		s_window->Maximize();
	}
	ImGui::SameLine();
	
	// Set close color
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 1.0f, 0.3f, 0.3f, 0.2f });
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, { 1.0f, 0.3f, 0.3f, 0.4f });

	// Close
	if (Core::ImGuiXtra::ImageButton("Close", s_closeIcon, delta)) {
		s_window->Close();
	}

	// Reset button colors
	ImGui::PopStyleColor(3);
	ImGui::PopStyleVar(2);

	// End and reset window
	ImGui::End();
	ImGui::PopStyleColor(4);
	ImGui::PopStyleVar();
}

void UITitleBar::Shutdown() {

}
