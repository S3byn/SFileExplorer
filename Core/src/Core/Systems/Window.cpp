#include "Window.h"
#include <GLFW/glfw3.h>

static bool s_glfwInit = false;

Core::Window::Window(const char* title, int width, int height) : m_width(width), m_height(height) {
	if (!s_glfwInit) {
		glfwInit();
		s_glfwInit = true;
	}

	GLFWwindow* window = glfwCreateWindow(width, height, title, nullptr, nullptr);
	m_handle = window;

	glfwMakeContextCurrent(window);

	glfwSwapInterval(1);
}

void Core::Window::Update() {
	glfwSwapBuffers(static_cast<GLFWwindow*>(m_handle));

	glfwPollEvents();
}

Core::Window::~Window() {
	glfwDestroyWindow(static_cast<GLFWwindow*>(m_handle));
	m_handle = nullptr;
}

bool Core::Window::ShouldClose() {
	return glfwWindowShouldClose(static_cast<GLFWwindow*>(m_handle));
}

std::unique_ptr<Core::Window> Core::CreateWindow(const char* title, int width, int height) {
	return std::make_unique<Core::Window>(title, width, height);
}
