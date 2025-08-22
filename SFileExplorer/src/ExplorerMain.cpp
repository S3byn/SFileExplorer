#include "ExplorerMain.h"

#include "UIComponents/UITitleBar.h"
#include "UIComponents/UIMenuBar.h"
#include "UIComponents/UIMainView.h"
#include "UIComponents/UIQuickAccess.h"

bool g_updateFiles = false;
static Core::sPtr<Explorer::Directory> s_files;
static Core::sPtr<UIMenuBar> s_uiMenuBar;

void ExplorerApp::AppStart() {
	//Get IO
	auto& io = ImGui::GetIO();

	#ifdef _DEBUG
	io.IniFilename = nullptr;
	#endif

	Core::ImGuiManager::AddFont("Default", 17.0f, "Assets/Fonts/Inter_18pt-Regular.ttf", true);
	Core::ImGuiManager::AddFont("DefaultBold", 17.0f, "Assets/Fonts/Inter_18pt-Medium.ttf");
	
	Core::AssetManager::LoadTexturesInPath("Assets/Textures");

	Core::UIManager::AddComponent<UITitleBar>();
	Core::UIManager::AddComponent<UIMenuBar>();
	Core::UIManager::AddComponent<UIQuickAccess>();
	Core::UIManager::AddComponent<UIMainView>();

	s_files = Core::UIManager::GetComponent<UIMainView>()->files;
	s_uiMenuBar = Core::UIManager::GetComponent<UIMenuBar>();
}


void ExplorerApp::AppUpdate(float delta) {
	if (g_updateFiles) {
		s_files->Reload();
		strcpy_s(s_uiMenuBar->directoryBuffer, sizeof(s_uiMenuBar->directoryBuffer), s_files->path.string().c_str());
		g_updateFiles = false;
	}
}

void ExplorerApp::AppEnd() {

}
