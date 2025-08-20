#pragma once
#include <AppCore.h>
#include "../Explorer/DirectoryUtils.h"

class UIMainView : public Core::UIComponent {
public:
	void Init() override;
	void Update(float delta) override;
	void Shutdown() override;
	
	Core::sPtr<Explorer::Directory> files;
	int mainMenuIndex = -1;

	REGISTER_COMPONENT(UIMainView)
};

