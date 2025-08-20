#include "UIQuickAccess.h"
#include "UIMenuBar.h"

static Core::Window* s_window;
static Core::sPtr<UIMenuBar> s_uiMenuBar;
static float s_mainViewOffset = 0.0f;

void UIQuickAccess::Init() {
	s_window = &Core::App::GetWindow();
	s_uiMenuBar = Core::UIManager::GetComponent<UIMenuBar>();
	s_mainViewOffset = (float)s_window->GetTitleBarHeight() + s_uiMenuBar->menuBarHeight - 2.0f;
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

	ImGui::PopStyleColor(5);
	ImGui::PopStyleVar(2);
}

void UIQuickAccess::Shutdown() {

}
