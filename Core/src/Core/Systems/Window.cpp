#include "Window.h"

// GLFW includes and configuration
#define GLFW_EXPOSE_NATIVE_WIN32
#define GLFW_EXPOSE_NATIVE_WGL
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

// Window API Includes
#include <Windowsx.h>
#include <dwmapi.h>

// Define statics for class
static bool s_glfwInit = false;
static int s_titleHeight, s_controlWidth;

LRESULT CALLBACK WinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

Core::Window::Window(const char* title, int width, int height) : m_width(width), m_height(height) {
	// Initalize GLFW
	if (!s_glfwInit) {
		glfwInit();
		s_glfwInit = true;
	}
	
	// Set the static variables
	s_titleHeight = m_titleHeight;
	s_controlWidth = m_controlWidth;

	// Create window
	GLFWwindow* window = glfwCreateWindow(width, height, title, nullptr, nullptr);
	m_handle = window;

	// Make context link to the window
	glfwMakeContextCurrent(window);

	// Get native handle for window customization
	HWND hwnd = glfwGetWin32Window(window);

	// Store original GLFW WinProc
	LONG_PTR originalProc = GetWindowLongPtr(hwnd, GWLP_WNDPROC);
	SetWindowLongPtr(hwnd, GWLP_USERDATA, originalProc);
	SetWindowLongPtr(hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(WinProc));

	// Remove title bar but keep resizable border
	LONG_PTR style = GetWindowLongPtr(hwnd, GWL_STYLE);
	style &= ~(WS_CAPTION | WS_THICKFRAME | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX);
	SetWindowLongPtr(hwnd, GWL_STYLE, style);

	LONG_PTR exStyle = GetWindowLongPtr(hwnd, GWL_EXSTYLE);
	exStyle &= ~(WS_EX_CLIENTEDGE | WS_EX_STATICEDGE | WS_EX_DLGMODALFRAME);
	SetWindowLongPtr(hwnd, GWL_EXSTYLE, exStyle);

	// Rounded corners (Windows 11)
	int preference = 2; // DWMWCP_ROUND
	DwmSetWindowAttribute(hwnd, 33, &preference, sizeof(preference));

	// No glass extension
	MARGINS margins = { 0, 0, 0, 0 };
	DwmExtendFrameIntoClientArea(hwnd, &margins);

	// Force style update
	SetWindowPos(hwnd, NULL, 0, 0, 0, 0,
		SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);

	//Set window to vsync
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

void Core::Window::SetTitleBarSize(int height, int controlWidth) { 
	m_titleHeight = height;
	s_titleHeight = m_titleHeight;

	m_controlWidth = controlWidth;
	s_controlWidth = m_controlWidth;
}

Core::uPtr<Core::Window> Core::MakeWindow(const char* title, int width, int height) {
	return SetUPtr<Core::Window>(title, width, height);
}


LRESULT CALLBACK WinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
		case WM_NCHITTEST: {
			POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
			ScreenToClient(hWnd, &pt);
			RECT rect;
			GetClientRect(hWnd, &rect);

			const int border = 8;

			// Resizable edges
			if (pt.y < border) {
				if (pt.x < border) return HTTOPLEFT;
				if (pt.x > rect.right - border) return HTTOPRIGHT;
				return HTTOP;
			} 
			else if (pt.y > rect.bottom - border) {
				if (pt.x < border) return HTBOTTOMLEFT;
				if (pt.x > rect.right - border) return HTBOTTOMRIGHT;
				return HTBOTTOM;
			} 
			else if (pt.x < border){
				return HTLEFT;
			}
			else if (pt.x > rect.right - border){
				return HTRIGHT;
			}

			// Drag zone: top 40px
			if (pt.y < s_titleHeight && pt.x < rect.right - s_controlWidth) return HTCAPTION;

			return HTCLIENT;
		}

		case WM_DESTROY:
			PostQuitMessage(0);
		return 0;

		default: {
			auto originalProc = reinterpret_cast<WNDPROC>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
			return CallWindowProc(originalProc, hWnd, msg, wParam, lParam);
		}
	}
}
