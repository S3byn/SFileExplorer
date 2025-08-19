#pragma once

#include "Core/Systems/Window.h"
#include "Core/Systems/ImGuiManager.h"
#include "Core/Systems/UIManager.h"
#include "Core/Systems/AssetManager.h"
#include "Core/Utilities/System.h"
#include "Core/Core.h"

namespace Core {

	class App {
	public:
		App(const char* name) : m_appName(name) {};
		virtual ~App() = default;

		void Init();
		virtual void AppStart() = 0;

		void Run();
		virtual void AppUpdate(float delta) = 0;

		void Shutdown();
		virtual void AppEnd() = 0;



		bool IsRunning();

		inline static Window& GetWindow() { return *Get().m_window; }

		static void SetClearColor(float r, float g, float b) { Get().m_clearColor = { r, g, b }; }
		static void SetClearColor(const glm::vec3& color) { Get().m_clearColor = color; }

	private:
		static App& Get();
		
	private:
		uPtr<Window> m_window = nullptr;
		uPtr<ImGuiManager> m_imManager = nullptr;

		glm::vec3 m_clearColor = { 0.0f,0.0f,0.0f };

		const char* m_appName;

		float m_deltaTime = 0.0f;
		float m_lastFrame = 0.0f;
	};

	//Defined client-side 
	App* CreateApp();
	
}

