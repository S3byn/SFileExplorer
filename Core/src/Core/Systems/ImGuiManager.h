#pragma once

#include <imgui.h>
#include <glm/glm.hpp>

namespace Core {

	class ImGuiManager {
	public:
		ImGuiManager();
		~ImGuiManager();

		static void DrawBegin(const glm::vec3& color);

		static void DrawEnd();
	};

}


