#pragma once

#include <memory>

namespace Core {

	template<typename T>
	using uPtr = std::unique_ptr<T>;

	template<typename T, typename... Args>
	uPtr<T> SetUPtr(Args&&... args) {
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using sPtr = std::shared_ptr<T>;

	template<typename T, typename... Args>
	sPtr<T> SetSPtr(Args&&... args) {
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

}

