#pragma once

#include <memory>

#include "Window.h"

namespace Core {

	class App {
	public:
		void Init();
		virtual void AppStart() = 0;

		void Run();
		virtual void AppUpdate() = 0;

		void Shutdown();
		virtual void AppEnd() = 0;

		bool IsRunning();

	private:
		std::unique_ptr<Window> m_window = nullptr;
	};

	//Defined client-side 
	App* CreateApp();
	
}

