#pragma once

#include <string>

#define REGISTER_COMPONENT(type) static std::string GetName() { return #type; } 

namespace Core {
	
	class UIComponent {
	public:
		UIComponent() {}
		virtual ~UIComponent() {}

		virtual void Init() = 0;
		virtual void Update() = 0;
		virtual void Shutdown() = 0;
	};

}


