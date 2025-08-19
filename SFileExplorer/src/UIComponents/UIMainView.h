#pragma once
#include <AppCore.h>
#include "../Explorer/DirectoryUtils.h"

class UIMainView : public Core::UIComponent {
public:
	void Init() override;
	void Update(float delta) override;
	void Shutdown() override;
	
	REGISTER_COMPONENT(UIMainView)

	Core::sPtr<Explorer::Directory> files;
	Explorer::Path currentDirectory = "C:\\";
	
	int mainMenuIndex = -1;
};

