#include "ImGuiManager.h"

#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <GLFW/glfw3.h>

#include "Core/Systems/App.h"

Core::ImGuiManager::ImGuiManager() {
	IMGUI_CHECKVERSION();

	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;

	ImGui::StyleColorsDark();

	GLFWwindow* window = static_cast<GLFWwindow*>(App::GetWindow().GetHandle());

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 410");
}

Core::ImGuiManager::~ImGuiManager() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void Core::ImGuiManager::DrawBegin(const glm::vec3& color) {
	glClearColor(color.r, color.g, color.b, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void Core::ImGuiManager::DrawEnd() {
	ImGuiIO& io = ImGui::GetIO();
	io.DisplaySize = ImVec2(
		(float)App::GetWindow().GetWidth(),
		(float)App::GetWindow().GetHeight()
	);

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
