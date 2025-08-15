#pragma once
#include <AppCore.h>

class UITitleBar : public Core::UIComponent {
public:
	void Init() override;
	void Update() override;
	void Shutdown() override;

	REGISTER_COMPONENT(UITitleBar)
};

