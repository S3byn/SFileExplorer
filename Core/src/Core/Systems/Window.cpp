#include "Window.h"

// Window API Includes
#include <Windowsx.h>
#include <dwmapi.h>

//Open GL
#include <glad/glad.h>

// GLFW includes and configuration
#define GLFW_EXPOSE_NATIVE_WIN32
#define GLFW_EXPOSE_NATIVE_WGL
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include "Core/Systems/App.h"

// Define statics for class
static bool s_glfwInit = false;
static int s_titleHeight, s_controlWidth;

static HWND s_hwnd;

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
	s_hwnd = glfwGetWin32Window(window);

	// Store original GLFW WinProc
	LONG_PTR originalProc = GetWindowLongPtr(s_hwnd, GWLP_WNDPROC);
	SetWindowLongPtr(s_hwnd, GWLP_USERDATA, originalProc);
	SetWindowLongPtr(s_hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(WinProc));

	LONG style = GetWindowLong(s_hwnd, GWL_STYLE);
	//style &= ~(WS_CAPTION);
	SetWindowLong(s_hwnd, GWL_STYLE, style);

	MARGINS margins = { 0, 0, 0, 0 };
	DwmExtendFrameIntoClientArea(s_hwnd, &margins);

	SetWindowPos(s_hwnd, NULL, 0,0,0,0,
		SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE | SWP_FRAMECHANGED); 

	//Set window to vsync
	glfwSwapInterval(1);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		// Handle error
	}
}

void Core::Window::Update() {
	auto window = static_cast<GLFWwindow*>(m_handle);

	glfwGetWindowSize(window, &m_width, &m_height);

	glfwSwapBuffers(window);
	glfwPollEvents();
}

void Core::Window::Minimize() {
	ShowWindow(s_hwnd, SW_MINIMIZE);
}

void Core::Window::Maximize() {
	WINDOWPLACEMENT wp = { sizeof(wp) };
	GetWindowPlacement(s_hwnd, &wp);
	ShowWindow(s_hwnd, wp.showCmd == SW_SHOWMAXIMIZED ? SW_RESTORE : SW_MAXIMIZE);
}

void Core::Window::Close() {
	PostQuitMessage(0);
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

bool Core::Window::GetMaximized() {
	return IsMaximized(s_hwnd);
}

LRESULT CALLBACK WinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
		
		case WM_NCCALCSIZE: {
			if (wParam == TRUE) {
				// Call the original window procedure to get default layout
				WNDPROC originalProc = reinterpret_cast<WNDPROC>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
				LRESULT res = CallWindowProc(originalProc, hWnd, msg, wParam, lParam);

				// Detect if window is maximized
				WINDOWPLACEMENT wp = { sizeof(WINDOWPLACEMENT) };
				GetWindowPlacement(hWnd, &wp);
				const bool isMaximized = (wp.showCmd == SW_SHOWMAXIMIZED);

				// Adjust the top margin manually
				int topOffset = GetSystemMetrics(SM_CYCAPTION);
					
				if (!isMaximized) {
					topOffset += (GetSystemMetrics(SM_CYFRAME) + GetSystemMetrics(SM_CXPADDEDBORDER) - 1);
				}

				NCCALCSIZE_PARAMS* sz = reinterpret_cast<NCCALCSIZE_PARAMS*>(lParam);
				

				sz->rgrc[0].top -= topOffset;

				return 0;
			}
			break;
		}

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
			break;
		}
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
		break;

		default: {
			auto originalProc = reinterpret_cast<WNDPROC>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
			return CallWindowProc(originalProc, hWnd, msg, wParam, lParam);
		}
	}
	return 0;
}
