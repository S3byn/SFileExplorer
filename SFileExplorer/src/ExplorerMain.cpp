#include "ExplorerMain.h"

#include "UITitleBar.h"

void ExplorerApp::AppStart() {
	//Get IO
	auto& io = ImGui::GetIO();
	#ifdef _DEBUG
	io.IniFilename = nullptr;
	#endif
	
	Core::ImGuiManager::AddFont("Default", 17.0f, "Assets/Fonts/Inter_18pt-Regular.ttf", true);
	Core::ImGuiManager::AddFont("DefaultBold", 17.0f, "Assets/Fonts/Inter_18pt-Medium.ttf");

	Core::UIManager::AddComponent<UITitleBar>();
}

void ExplorerApp::AppUpdate() {

}

void ExplorerApp::AppEnd() {

}
