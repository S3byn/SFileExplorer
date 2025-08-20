#pragma once
#include <AppCore.h>

class UIQuickAccess : public Core::UIComponent {
public:
	void Init() override;
	void Update(float delta) override;
	void Shutdown() override;

	float width = 256.0f;

	REGISTER_COMPONENT(UIQuickAccess)
private:
	float initialized = false;
	bool clicked = false;

};

