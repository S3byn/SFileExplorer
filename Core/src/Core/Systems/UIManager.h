#pragma once

#include "Core/Resources/UIComponent.h"
#include "Core/Core.h"

#include <unordered_map>
#include <string>

namespace Core {

	class UIManager {
	public:
		template <typename T>
		static void AddComponent() {
			auto instance = SetSPtr<T>();
			auto& map = Get().m_components;
			if (map.find(instance->GetName()) == map.end()) {
				map.emplace(instance->GetName(), instance);
				instance->Init();
			}
		}
		
		template <typename T>
		static sPtr<UIComponent> GetComponent() {
			auto it = Get().m_components.find(T::GetName());
			if (it != Get().m_components.end()) {
				return it->second;
			}
		}

		template <typename T>
		static void RemoveComponent() {
			auto it = Get().m_components.find(T::GetName());
			if (it != Get().m_components.end()) {
				it->second->Shutdown();
				Get().m_components.erase(it);
			}
		}

		static void Init();
		static void Update();
		static void Shutdown();
	private:
		UIManager() {}
		~UIManager() {}
		
		static UIManager& Get();

	private:
		std::unordered_map<std::string, sPtr<UIComponent>> m_components;
	};

}

