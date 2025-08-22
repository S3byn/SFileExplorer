#include "UIMainView.h"
#include "UIMenuBar.h"
#include "UIQuickAccess.h"

#include <array>

static Core::Window* s_window;
static Core::sPtr<UIMenuBar> s_uiMenuBar;
static float s_mainViewOffset = 0.0f;
static float* s_quickAccessWidth;

//Icons
static Core::sPtr<Core::Texture> s_directoryIcon;
static Core::sPtr<Core::Texture> s_fileIcon;

static constexpr float s_padding = 25.0f;

extern bool g_updateFiles;

static std::array<glm::vec4, 4> s_headerColors = {
	glm::vec4(0.0f, 0.0f, 0.0f, 0.0f),
	glm::vec4(0.0f, 0.0f, 0.0f, 0.0f),
	glm::vec4(0.0f, 0.0f, 0.0f, 0.0f),
	glm::vec4(0.0f, 0.0f, 0.0f, 0.0f)
};

static std::array<float, 4> s_columnPosition = { 0.0f };
static std::array<float, 4> s_columnsWidth = { 0.0f };

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
	ImGui::TableSetupColumn("Name");
	ImGui::TableSetupColumn("Type");
	ImGui::TableSetupColumn("Created Time");
	ImGui::TableSetupColumn("Size");
	
	ImGui::TableHeadersRow();

	// Get the cursor position
	ImVec2 cursor = ImGui::GetMousePos();
	// Handle animations for the header
	for (int i = 0; i < ImGui::TableGetColumnCount(); i++) {
		// Define each table index
		ImGui::TableSetColumnIndex(i);
		ImGui::TableHeader(ImGui::TableGetColumnName(i));
		// Set their positions
		s_columnPosition[i] = ImGui::GetCursorPosX();
		s_columnsWidth[i] = ImGui::GetColumnWidth(i);
		
		// Get the normalized column position
		float normalizedColumnPosition = s_columnPosition[i] + ImGui::GetWindowPos().x;

		// Check if we're colliding with a resize control to change the cursor
		if (cursor.x > normalizedColumnPosition + s_columnsWidth[i] - 1 && 
			cursor.x < normalizedColumnPosition + s_columnsWidth[i] + 8 && 
			cursor.y > ImGui::GetWindowPos().y + 1 && 
			cursor.y < ImGui::GetWindowPos().y + 23) {
			Core::System::SetCursor(Core::Cursor::HResize);
		}

		// Table header animation
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

		// Set the appropriate color based on the animation
		ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, ImGui::ColorConvertFloat4ToU32(Core::GLMToImVec(s_headerColors[i])), i);
	}
	ImGui::EndTable();

	// Display files
	int i = 0;
	bool selected = false;
	for (const auto& file : *files) {
		// If that particular index is selected permanently switch the button background
		ImGui::PushStyleColor(ImGuiCol_Button, mainMenuIndex == i ? Core::ImGuiXtra::HexColor(0xBA9EFF, 0.2f) : ImVec4(0.0f, 0.0f, 0.0f, 0.0f ));

		// Render the button
		Core::ImGuiXtra::Button(("##" + file.name).c_str(), { s_window->GetWidth() - ImGui::GetStyle().WindowPadding.x, s_directoryIcon->height }, delta);
		
		// Handle selection logic
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
		// Render the icons and text
		
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() - s_window->GetWidth() + s_columnPosition[0]);
		Core::ImGuiXtra::Image(file.isDirectory ? s_directoryIcon : s_fileIcon);
		
		ImGui::SameLine();
		std::string fileName = Core::ClipTextToWidth(file.name, s_columnsWidth[0] - s_padding);
		ImGui::Text(fileName.c_str());

		ImGui::SameLine();
		ImGui::SetCursorPosX(s_columnPosition[1]);
		std::string fileYypeDesc = Core::ClipTextToWidth(file.typeDesc, s_columnsWidth[1] - s_padding);
		ImGui::Text(fileYypeDesc.c_str());

		i++;
	}
	
	ImGui::PopStyleColor(8);
	ImGui::PopStyleVar(2);
}

void UIMainView::Shutdown() {

}
