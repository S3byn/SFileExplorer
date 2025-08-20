#include "UIMenuBar.h"
#include "UIMainView.h"

#include <iostream>

static Core::Window* s_window;

//Menu Bar Icons
static Core::sPtr<Core::Texture> s_menuIcon;
static Core::sPtr<Core::Texture> s_undoIcon;
static Core::sPtr<Core::Texture> s_redoIcon;
static Core::sPtr<Core::Texture> s_prevIcon;
static Core::sPtr<Core::Texture> s_refreshIcon;

extern bool g_updateFiles;

void UIMenuBar::Init() {
	s_window = &Core::App::GetWindow();

	s_menuIcon = Core::AssetManager::GetTexture("menuIcon.png");
	s_undoIcon = Core::AssetManager::GetTexture("backArrowIcon.png");
	s_redoIcon = Core::AssetManager::GetTexture("fowardArrowIcon.png");
	s_prevIcon = Core::AssetManager::GetTexture("upArrowIcon.png");
	s_refreshIcon = Core::AssetManager::GetTexture("refreshIcon.png");
}

void UIMenuBar::Update(float delta) {
	// Set Window BG
	ImGui::PushStyleColor(ImGuiCol_WindowBg, Core::ImGuiXtra::HexColor(0x262051, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_Border, Core::ImGuiXtra::HexColor(0x403363, 1.0f));

	// Set Button Colors
	ImGui::PushStyleColor(ImGuiCol_Button, { 0.0f, 0.0f, 0.0f, 0.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, Core::ImGuiXtra::HexColor(0xBA9EFF, 0.2f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, Core::ImGuiXtra::HexColor(0xBA9EFF, 0.4f));
	ImGui::PushStyleColor(ImGuiCol_FrameBg, Core::ImGuiXtra::HexColor(0x4E4182, 1.0f));

	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 2.0f, 0.0f });
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, { 0.0f, 0.0f });
	ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 6.0f);

	// Set Create Menu bar window
	ImGui::SetNextWindowPos({ -1.0f, (float)s_window->GetTitleBarHeight() - 1.0f });
	ImGui::SetNextWindowSize({ (float)s_window->GetWidth() + 2, menuBarHeight });
	Core::ImGuiXtra::Window menuBar(UIMenuBar::GetName().c_str(), nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar);
	
	ImGui::SetCursorPos({ 2.0f, 0.0f });

	auto& files = Core::UIManager::GetComponent<UIMainView>()->files;
	// Menu Icon
	if (Core::ImGuiXtra::ImageButton("Menu Icon", s_menuIcon, delta)) {
		// Do nothing for now
	}
	ImGui::SameLine();

	// Undo Icon
	if (Core::ImGuiXtra::ImageButton("Undo Icon", s_undoIcon, delta)) {
		// Do nothing for now
	}
	ImGui::SameLine();

	// Redo Icon
	if (Core::ImGuiXtra::ImageButton("Redo Icon", s_redoIcon, delta)) {
		// Do nothing for now
	}
	ImGui::SameLine();

	// Prev Icon
	if (Core::ImGuiXtra::ImageButton("Prev Icon", s_prevIcon, delta)) {
		
		auto path = files->path.lexically_normal();

		if (!path.has_filename() && path.has_parent_path()) {
			path = path.parent_path();
		}
		else if (path.has_filename()) {
			path = path.parent_path();
		}
		
		files->path = path;
		g_updateFiles = true;
	}
	ImGui::SameLine();

	// Refresh Icon
	if (Core::ImGuiXtra::ImageButton("Refresh Icon", s_refreshIcon, delta)) {
		// Do nothing for now
	}
	ImGui::SameLine();

	// Search bar
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, {6.0f,3.0f});
	ImGui::PushStyleColor(ImGuiCol_TextDisabled, {1.0f, 1.0f, 1.0f, 0.5f});

	float searchBarWidth = s_window->GetWidth() * 0.25f;

	ImGui::SetCursorPos({ImGui::GetCursorPosX() + 2.0f, 5.0f});
	ImGui::SetNextItemWidth(s_window->GetWidth() - ImGui::GetCursorPosX() - searchBarWidth - ImGui::GetStyle().ItemSpacing.x * 2.0f);
	if (ImGui::InputText("##Directory", directoryBuffer, IM_ARRAYSIZE(directoryBuffer), ImGuiInputTextFlags_EnterReturnsTrue)) {
		Explorer::Path currentEvaluatedPath = directoryBuffer;
		if (Explorer::IsDirectory(currentEvaluatedPath)) {
			files->path = currentEvaluatedPath;
			g_updateFiles = true;
		}
		else {
			strcpy_s(directoryBuffer, sizeof(directoryBuffer), files->path.string().c_str());
		}
	}
	if (ImGui::IsItemHovered()) {
		Core::System::SetCursor(Core::Cursor::Text);
	}

	ImGui::SameLine();
		
	std::string searchHint = "Search " + files->path.filename().string();

	ImGui::SetCursorPos({ImGui::GetCursorPosX() + 2.0f, 5.0f});
	ImGui::SetNextItemWidth(searchBarWidth - ImGui::GetStyle().ItemSpacing.x);
	if (ImGui::InputTextWithHint("##Search", searchHint.c_str(), searchBuffer, IM_ARRAYSIZE(searchBuffer), ImGuiInputTextFlags_EnterReturnsTrue)) {
		// Do nothing for now
	}
	if (ImGui::IsItemHovered()) {
		Core::System::SetCursor(Core::Cursor::Text);
	}

	ImGui::PopStyleColor(7);
	ImGui::PopStyleVar(4);
}

void UIMenuBar::Shutdown() {

}
