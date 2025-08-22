#include "ImGuiUtilities.h"
#include <unordered_map>
#include <string>
#include "Math.h"

ImVec2 Core::GLMToImVec(const glm::vec2& vec) {
	return { vec.x, vec.y };
}

ImVec4 Core::GLMToImVec(const glm::vec4& vec) {
	return { vec.r, vec.g, vec.b, vec.a };
}

glm::vec2 Core::ImVecToGLM(const ImVec2& vec) {
	return { vec.x, vec.y };
}

glm::vec4 Core::ImVecToGLM(const ImVec4& vec) {
	return { vec.x, vec.y, vec.z, vec.w };
}

std::string Core::ClipTextToWidth(const std::string& text, float maxWidth, const char* ellipsis) {
	const ImVec2 ellipsisSize = ImGui::CalcTextSize(ellipsis);
	const float availableWidth = maxWidth - ellipsisSize.x;

	size_t clippedLen = 0;
	float totalWidth = 0.0f;

	for (size_t i = 0; i < text.size(); ++i) {
		ImVec2 charSize = ImGui::CalcTextSize(std::string(1, text[i]).c_str());
		totalWidth += charSize.x;
		if (totalWidth > availableWidth)
			break;
		clippedLen = i + 1;
	}

	if (clippedLen < text.size())
		return text.substr(0, clippedLen) + ellipsis;
	else
		return text;
}

bool Core::ImGuiXtra::Button(const std::string& label, const glm::vec2& size, float deltaTime) {
	ImGui::PushID(label.c_str());

	glm::vec2 pos = Core::ImVecToGLM(ImGui::GetCursorScreenPos());
	glm::vec2 end = pos + size;
	ImDrawList* drawList = ImGui::GetWindowDrawList();
	ImGuiID id = ImGui::GetID(label.c_str());

	// Persistent animation states
	static std::unordered_map<ImGuiID, float> hoverAnim;
	static std::unordered_map<ImGuiID, float> clickAnim;

	float& t = hoverAnim[id];
	float& c = clickAnim[id];

	bool hovered = ImGui::IsMouseHoveringRect(Core::GLMToImVec(pos), Core::GLMToImVec(end));
	bool pressed = hovered && ImGui::IsMouseDown(ImGuiMouseButton_Left);

	// Animate hover
	t = hovered ? std::min(t + deltaTime * 30.0f, 1.0f) : std::max(t - deltaTime * 6.0f, 0.0f);

	// Animate click
	float targetClick = pressed ? 1.0f : 0.0f;
	c += (targetClick - c) * deltaTime * 12.0f;

	// Style-aware colors
	auto& style = ImGui::GetStyle();
	const glm::vec4& baseColor   = Core::ImVecToGLM(style.Colors[ImGuiCol_Button]);
	const glm::vec4& hoverColor  = Core::ImVecToGLM(style.Colors[ImGuiCol_ButtonHovered]);
	const glm::vec4& activeColor = Core::ImVecToGLM(style.Colors[ImGuiCol_ButtonActive]);
	glm::vec4 finalColor = Core::Math::Lerp(Core::Math::Lerp(baseColor, hoverColor, t), activeColor, c);

	// Press scale
	glm::vec2 center = (pos + end) * 0.5f;
	float scaleX = 1.0f - c * 0.0f; // gentler on X
	float scaleY = 1.0f - c * 0.1f; // stronger on Y
	glm::vec2 halfSize = {size.x * 0.5f * scaleX, size.y * 0.5f * scaleY};
	glm::vec2 newPos = center - halfSize;
	glm::vec2 newEnd = center + halfSize;

	// Draw
	drawList->AddRectFilled(Core::GLMToImVec(newPos), Core::GLMToImVec(newEnd), ImColor(Core::GLMToImVec(finalColor)), ImGui::GetStyle().FrameRounding);

	if (label.find("##") == std::string::npos) {
		glm::vec textPos = center - Core::ImVecToGLM(ImGui::CalcTextSize(label.c_str())) * 0.5f;
		drawList->AddText(Core::GLMToImVec(textPos),
			ImColor(ImGui::GetStyle().Colors[ImGuiCol_Text]), label.c_str());
	}

	// Input
	ImGui::SetCursorScreenPos(Core::GLMToImVec(pos));
	bool clicked = ImGui::InvisibleButton(label.c_str(), Core::GLMToImVec(size));

	ImGui::PopID();
	return clicked;
}

bool Core::ImGuiXtra::ImageButton(const std::string& label, const Core::sPtr<Core::Texture>& texture, float delta) {
	bool result = Button(("##" + label).c_str(), {(float)texture->width, (float)texture->height}, delta);

	ImGui::SameLine();
	ImGui::SetCursorPosX(ImGui::GetCursorPosX() - (float)texture->width - ImGui::GetStyle().ItemSpacing.x);
	ImGui::Image((ImTextureID)texture->id, {(float)texture->width, (float)texture->height});
	return result;
}

void Core::ImGuiXtra::Image(const Core::sPtr<Core::Texture>& texture, const glm::vec2& size) {
	if (size == glm::vec2(0.0f, 0.0f)) {
		ImGui::Image((ImTextureID)texture->id, {(float)texture->width, (float)texture->height});
	}
	else {
		ImGui::Image((ImTextureID)texture->id, { size.x, size.y });
	}
}
