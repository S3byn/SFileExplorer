#pragma once

#include <imgui.h>
#include <glm/glm.hpp>

#include <filesystem>

namespace Core {

	class ImGuiManager {
	public:
		ImGuiManager();
		~ImGuiManager();

		static void DrawBegin(const glm::vec3& color);

		static void DrawEnd();

		static void AddFont(const std::string& name, float size, const std::filesystem::path & filePath, bool setDefault = false);

		static ImFont* GetFont(const std::string& name);
	private:
		static ImGuiManager& Get();
	};

}


