#include "ExplorerMain.h"

#include "UIComponents/UITitleBar.h"
#include "UIComponents/UIMenuBar.h"

void ExplorerApp::AppStart() {
	//Get IO
	auto& io = ImGui::GetIO();
static float posX = 0.0f;

	#ifdef _DEBUG
	io.IniFilename = nullptr;
	#endif

	Core::ImGuiManager::AddFont("Default", 17.0f, "Assets/Fonts/Inter_18pt-Regular.ttf", true);
	Core::ImGuiManager::AddFont("DefaultBold", 17.0f, "Assets/Fonts/Inter_18pt-Medium.ttf");
	
	Core::AssetManager::LoadTexturesInPath("Assets/Textures");

	Core::UIManager::AddComponent<UIMenuBar>();
	Core::UIManager::AddComponent<UITitleBar>();
}


void ExplorerApp::AppUpdate(float delta) {

}

void ExplorerApp::AppEnd() {

}
