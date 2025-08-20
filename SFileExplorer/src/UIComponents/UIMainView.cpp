#include "UIMainView.h"
#include "UIMenuBar.h"
#include "UIQuickAccess.h"

#include <iostream>

static Core::Window* s_window;
static Core::sPtr<UIMenuBar> s_uiMenuBar;
static float s_mainViewOffset = 0.0f;
static float* s_quickAccessWidth;

//Icons
static Core::sPtr<Core::Texture> s_directoryIcon;
static Core::sPtr<Core::Texture> s_fileIcon;

extern bool g_updateFiles;

static std::vector<glm::vec4> s_headerColors = {
	{ 0.0f, 0.0f, 0.0f, 0.0f },
	{ 0.0f, 0.0f, 0.0f, 0.0f },
	{ 0.0f, 0.0f, 0.0f, 0.0f },
	{ 0.0f, 0.0f, 0.0f, 0.0f }
};

void UIMainView::Init() {
	s_window = &Core::App::GetWindow();
	s_uiMenuBar = Core::UIManager::GetComponent<UIMenuBar>();
	s_mainViewOffset = (float)s_window->GetTitleBarHeight() + s_uiMenuBar->menuBarHeight - 2.0f;
	s_quickAccessWidth = &Core::UIManager::GetComponent<UIQuickAccess>()->width;

	files = Core::SetSPtr<Explorer::Directory>("C:\\");
	strcpy_s(s_uiMenuBar->directoryBuffer, sizeof(s_uiMenuBar->directoryBuffer), files->path.string().c_str());

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

	ImGui::PushStyleColor(ImGuiCol_TableHeaderBg, {0,0,0,0});
	ImGui::PushStyleColor(ImGuiCol_HeaderHovered, {0,0,0,0});
	ImGui::PushStyleColor(ImGuiCol_HeaderActive, {0,0,0,0});

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 2.0f, 2.0f });
	ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 6.0f);

	// Create window
	ImGui::SetNextWindowPos({ *s_quickAccessWidth, s_mainViewOffset});
	ImGui::SetNextWindowSize({(float)s_window->GetWidth() - (float)*s_quickAccessWidth + 1.0f, (float)s_window->GetHeight() - s_mainViewOffset + 1.0f});
	Core::ImGuiXtra::Window mainView(UIMainView::GetName().c_str(), nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
	
	// Create table header for files
	ImGui::BeginTable("FileTable", 4, ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_NoBordersInBody);
	ImGui::TableSetupColumn(" Name");
	ImGui::TableSetupColumn("Type");
	ImGui::TableSetupColumn("Created Time");
	ImGui::TableSetupColumn("Size");
	
	ImGui::TableHeadersRow();

	for (int i = 0; i < ImGui::TableGetColumnCount(); i++) {
		ImGui::TableSetColumnIndex(i);
		ImGui::TableHeader(ImGui::TableGetColumnName(i));
		
		if (ImGui::IsItemHovered()) {
			auto color = Core::ImVecToGLM(Core::ImGuiXtra::HexColor(0xBA9EFF, 0.2f));
			if (ImGui::IsItemActive()) {
				 color = Core::ImVecToGLM(Core::ImGuiXtra::HexColor(0xBA9EFF, 0.4f));
			}
			s_headerColors[i] = Core::Math::Lerp(s_headerColors[i], color, delta*40);
		}
		else {
			s_headerColors[i] = Core::Math::Lerp(s_headerColors[i], {0, 0, 0, 0}, delta*10);
		}

		ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, ImGui::ColorConvertFloat4ToU32(Core::GLMToImVec(s_headerColors[i])), i);
	}

	ImGui::EndTable();

	int i = 0;
	bool selected = false;
	for (const auto& file : *files) {
		if(mainMenuIndex == i){
			ImGui::PushStyleColor(ImGuiCol_Button, Core::ImGuiXtra::HexColor(0xBA9EFF, 0.2f));
		}
		else {
			ImGui::PushStyleColor(ImGuiCol_Button, { 0.0f, 0.0f, 0.0f, 0.0f });
		}

		Core::ImGuiXtra::Button(("##" + file.name).c_str(), { s_window->GetWidth() - ImGui::GetStyle().WindowPadding.x, s_directoryIcon->height }, delta);
		
		if (ImGui::IsItemHovered()) {
			if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) || ImGui::IsMouseClicked(ImGuiMouseButton_Right)) {
				mainMenuIndex = i;
				selected = true;
			}
			if(ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
				if (file.isDirectory) {
					std::string backSlash = files->path.string().back() == '\\' ? "" : "\\";
					files->path += backSlash + file.name;
					g_updateFiles = true;
				}
				else {
					Explorer::OpenFile(file.path);
				}
				mainMenuIndex = -1;
			}
		}
		else if (!selected && ImGui::IsMouseClicked(ImGuiMouseButton_Left) || ImGui::IsMouseClicked(ImGuiMouseButton_Right)) {
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
	
	ImGui::PopStyleColor(8);
	ImGui::PopStyleVar(2);
}

void UIMainView::Shutdown() {

}
