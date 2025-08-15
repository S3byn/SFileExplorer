#pragma once

#include <memory>

namespace Core {

	class Window {
	public:
		Window(const char* title, int width, int height);
		~Window();

		void Update();

		void* GetHandle() const { return m_handle; }

		int GetWidth() const { return m_width; }
		int GetHeight() const { return m_height; }

		bool ShouldClose();

		void SetTitleBarSize(int height, int controlWidth);

	private:
		void* m_handle = nullptr;

		int m_width = 0, m_height = 0;
		int m_titleHeight = 40, m_controlWidth = 120;
	};

	std::unique_ptr<Window> MakeWindow(const char* title, int width, int height);
}

