#pragma once

#include <memory>

#include "Window.h"
#include "ImGuiManager.h"

namespace Core {

	class App {
	public:
		App(const char* name) : m_appName(name) {};
		~App() = default;

		void Init();
		virtual void AppStart() = 0;

		void Run();
		virtual void AppUpdate() = 0;

		void Shutdown();
		virtual void AppEnd() = 0;

		bool IsRunning();

		inline static Window& GetWindow() { return *Get().m_window; }

		static void SetClearColor(float r, float g, float b) { Get().m_clearColor = { r, g, b }; }
		static void SetClearColor(const glm::vec3& color) { Get().m_clearColor = color; }

	private:
		static App& Get();
		
	private:
		std::unique_ptr<Window> m_window = nullptr;
		std::unique_ptr<ImGuiManager> m_imManager = nullptr;

		glm::vec3 m_clearColor;

		const char* m_appName;
	};

	//Defined client-side 
	App* CreateApp();
	
}

