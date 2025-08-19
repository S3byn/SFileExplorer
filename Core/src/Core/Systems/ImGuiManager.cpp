#include "ImGuiManager.h"

#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <GLFW/glfw3.h>

#include "Core/Systems/App.h"
#include <unordered_map>

static Core::ImGuiManager* s_manager = nullptr;

static std::unordered_map<std::string, int> s_loadedFonts;
static int s_currentIndex = 0;


Core::ImGuiManager::ImGuiManager() {
	if (!s_manager) s_manager = this;

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

void Core::ImGuiManager::AddFont(const std::string& name, float size, const std::filesystem::path& filePath, bool setDefault) {
	ImGuiIO& io = ImGui::GetIO();
	auto font = io.Fonts->AddFontFromFileTTF(filePath.string().c_str(), size);

	s_loadedFonts.insert({name, s_currentIndex});
	s_currentIndex++;

	if (setDefault) {
		io.FontDefault = font;
	}
}

ImFont* Core::ImGuiManager::GetFont(const std::string& name) {
	ImGuiIO& io = ImGui::GetIO();
	return io.Fonts->Fonts[s_loadedFonts[name]];
}
