#pragma once
#include <AppCore.h>
#include "../Explorer/DirectoryUtils.h"

class UIQuickAccess : public Core::UIComponent {
public:
	void Init() override;
	void Update(float delta) override;
	void Shutdown() override;

	float width = 256.0f;

	std::vector<Explorer::File> pinnedFiles;
	int mainMenuIndex = -1;

	REGISTER_COMPONENT(UIQuickAccess)
private:
	bool clicked = false;
	
};

