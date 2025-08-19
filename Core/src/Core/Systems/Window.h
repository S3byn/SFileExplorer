#pragma once

#include "Core/Core.h"

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

		void Maximize();
		void Minimize();
		void Close();

		void SetTitleBarSize(int height, int controlWidth);
		int GetTitleBarHeight() const { return m_titleHeight; }
		int GetTitleBarControlWidth() const { return m_controlWidth; }

		bool GetMaximized();

	private:
		void* m_handle = nullptr;

		int m_width = 0, m_height = 0;
		int m_titleHeight = 40, m_controlWidth = 120;
	};

	uPtr<Window> MakeWindow(const char* title, int width, int height);
}

