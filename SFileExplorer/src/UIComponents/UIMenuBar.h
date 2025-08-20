#pragma once
#include <AppCore.h>

class UIMenuBar : public Core::UIComponent {
public:
	void Init() override;
	void Update(float delta) override;
	void Shutdown() override;

	float menuBarHeight = 33.0f;
	char directoryBuffer[256] = "";
	char searchBuffer[96] = "";

	REGISTER_COMPONENT(UIMenuBar)
};


