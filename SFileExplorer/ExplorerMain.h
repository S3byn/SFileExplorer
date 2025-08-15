#pragma once

#define CORE_ENTRY_POINT
#include <AppCore.h>

class ExplorerApp : public Core::App {
public:
	ExplorerApp() :
		App("SFileExplorer") { }

	void AppStart() override;
	void AppUpdate() override;
	void AppEnd() override;
};

Core::App* Core::CreateApp() {
	return new ExplorerApp();
}
