#pragma once

#include <glm/glm.hpp>
#include <imgui.h>
#include <Core/Core.h>
#include <Core/Resources/Texture.h>

namespace Core {
	ImVec2 GLMToImVec(const glm::vec2& vec);
	ImVec4 GLMToImVec(const glm::vec4& vec);

	glm::vec2 ImVecToGLM(const ImVec2& vec);
	glm::vec4 ImVecToGLM(const ImVec4& vec);
	
	std::string ClipTextToWidth(const std::string& text, float maxWidth, const char* ellipsis = "...");

	namespace ImGuiXtra {

		class Window {
		public:
			Window(const char* label, bool* show = (bool*)0, ImGuiWindowFlags flags = 0) {
				ImGui::Begin(label, show, flags);
			}
			~Window() {
				ImGui::End();
			}
		};

		inline ImVec4 HexColor(uint32_t color) {
			float r = ((color >> 24) & 0xFF) / 255.0f;
			float g = ((color >> 16) & 0xFF) / 255.0f;
			float b = ((color >> 8) & 0xFF) / 255.0f;
			float a = (color & 0xFF) / 255.0f;
			return { r, g, b, a };
		}

		inline ImVec4 HexColor(uint32_t color, float alpha) {
			float r = ((color >> 16) & 0xFF) / 255.0f;
			float g = ((color >> 8)  & 0xFF) / 255.0f;
			float b = (color & 0xFF) / 255.0f;
			return { r, g, b, alpha };
		}

		bool Button(const std::string& label, const glm::vec2& size, float deltaTime);

		bool ImageButton(const std::string& label, const Core::sPtr<Core::Texture>& texture, float delta);

		void Image(const Core::sPtr<Core::Texture>& texture, const::glm::vec2& size = { 0.0f, 0.0f });

	}
}
