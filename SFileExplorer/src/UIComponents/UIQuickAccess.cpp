#include "UIQuickAccess.h"
#include "UIMenuBar.h"
#include "UIMainView.h"

#include <iostream>

static Core::Window* s_window;
static Core::sPtr<UIMenuBar> s_uiMenuBar;
static float s_mainViewOffset = 0.0f;

static Core::sPtr<Core::Texture> s_directoryIcon;

static constexpr float s_padding = 25.0f;

extern bool g_updateFiles;

void UIQuickAccess::Init() {
	s_window = &Core::App::GetWindow();
	s_uiMenuBar = Core::UIManager::GetComponent<UIMenuBar>();
	s_mainViewOffset = (float)s_window->GetTitleBarHeight() + s_uiMenuBar->menuBarHeight - 2.0f;
	Explorer::Directory userDirectory(Explorer::GetUserDirectory());

	for (const auto& file : userDirectory) {
		if (file.name == "Desktop") {
			pinnedFiles.push_back(file);
		}
		else if (file.name == "Downloads") {
			pinnedFiles.push_back(file);
		}
		else if (file.name == "Pictures") {
			pinnedFiles.push_back(file);
		}
		else if (file.name == "Documents") {
			pinnedFiles.push_back(file);
		}
		else if (file.name == "Videos") {
			pinnedFiles.push_back(file);
		}
		else if (file.name == "Music") {
			pinnedFiles.push_back(file);
		}
	}

	s_directoryIcon = Core::AssetManager::GetTexture("directoryIcon.png");
	
}

void UIQuickAccess::Update(float delta) {
	float minWidth = (float)s_window->GetWidth() * 0.1f;
	float maxWidth = (float)s_window->GetWidth() * 0.5f;
	glm::vec2 mousePos = Core::ImVecToGLM(ImGui::GetMousePos());
	
	if (mousePos.x > width - 3 && mousePos.x < width + 3 && mousePos.y > s_mainViewOffset && mousePos.y < (float)s_window->GetHeight()) {
		Core::System::SetCursor(Core::Cursor::HResize);
		if (ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
			clicked = true;
		}
	}
	if (ImGui::IsMouseReleased(ImGuiMouseButton_Left)) {
		clicked = false;
	}
	if (clicked) {
		width = mousePos.x;
	}
	width = Core::Math::Clamp(width, minWidth, maxWidth);

	// Set Window BG
	ImGui::PushStyleColor(ImGuiCol_WindowBg, Core::ImGuiXtra::HexColor(0x181133, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_Border, Core::ImGuiXtra::HexColor(0x403363, 1.0f));

	ImGui::PushStyleColor(ImGuiCol_Button, { 0.0f, 0.0f, 0.0f, 0.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, Core::ImGuiXtra::HexColor(0xBA9EFF, 0.2f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, Core::ImGuiXtra::HexColor(0xBA9EFF, 0.4f));

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 2.0f, 2.0f });
	ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 6.0f);

	// Create window
	ImGui::SetNextWindowPos({ -1.0f, s_mainViewOffset});
	ImGui::SetNextWindowSize({ width + 2.0f, s_window->GetHeight() - s_mainViewOffset + 1.0f});
	Core::ImGuiXtra::Window mainView(UIQuickAccess::GetName().c_str(), nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
	
	// Render Shadow
	auto shadow = Core::AssetManager::GetTexture("shadow.png");
	Core::ImGuiXtra::Image(shadow, { ImGui::GetWindowWidth(), shadow->height });
	ImGui::SetCursorPos({ 0.0f, 0.0f });

	// Header text
	Core::ImGuiXtra::Image(Core::AssetManager::GetTexture("pinnedIcon.png"));
	ImGui::SameLine();
	ImGui::Text("Pinned Items");

	auto& files = Core::UIManager::GetComponent<UIMainView>()->files;

	// Render pinned files
	int i = 0;
	bool selected = false;
	for (const auto& file : pinnedFiles) {
		// If that particular index is selected permanently switch the button background
		ImGui::PushStyleColor(ImGuiCol_Button, mainMenuIndex == i ? Core::ImGuiXtra::HexColor(0xBA9EFF, 0.2f) : ImVec4(0.0f, 0.0f, 0.0f, 0.0f ));

		// Render the button
		Core::ImGuiXtra::Button(("##" + file.name).c_str(), { s_window->GetWidth() - ImGui::GetStyle().WindowPadding.x, s_directoryIcon->height }, delta);

		// Handle selection logic
		if (ImGui::IsItemHovered()) {
			if (ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
				files->path = file.path;
				g_updateFiles = true;
				mainMenuIndex = i;
				selected = true;
			}
		}
		else if (!selected && ImGui::IsMouseClicked(ImGuiMouseButton_Left) || ImGui::IsMouseClicked(ImGuiMouseButton_Right)) {
			mainMenuIndex = -1;
		}

		ImGui::PopStyleColor();
		ImGui::SameLine();
		// Render the icons and text

		ImGui::SetCursorPosX(ImGui::GetCursorPosX() - s_window->GetWidth());
		Core::ImGuiXtra::Image(s_directoryIcon);

		ImGui::SameLine();
		std::string fileName = Core::ClipTextToWidth(file.name, width - s_padding);
		ImGui::Text(fileName.c_str());

		i++;
	}


	ImGui::PopStyleColor(5);
	ImGui::PopStyleVar(2);
}

void UIQuickAccess::Shutdown() {

}
