#include "ExplorerMain.h"

#include "UITitleBar.h"

void ExplorerApp::AppStart() {
	Core::UIManager::AddComponent<UITitleBar>();
}

void ExplorerApp::AppUpdate() {

}

void ExplorerApp::AppEnd() {

}
