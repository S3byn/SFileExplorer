#include "UITitleBar.h"

void UITitleBar::Init() {
}

void UITitleBar::Update() {
	bool show = true;
	ImGui::ShowDemoWindow(&show);
}

void UITitleBar::Shutdown() {
}
