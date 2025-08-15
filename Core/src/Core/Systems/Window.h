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

	private:
		void* m_handle = nullptr;

		int m_width = 0, m_height = 0;
	};

	std::unique_ptr<Window> CreateWindow(const char* title, int width, int height);
}

