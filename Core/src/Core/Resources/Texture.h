#pragma once

#include <cstdint>
#include <string>

namespace Core {

	class Texture {
	public:
		Texture(const std::string& path) : path(path) {}
		~Texture();
		
		uint32_t id = 0;
		uint32_t width = 0, height = 0;
		std::string path;
		bool uploaded = false;

		bool IsReady() const { return uploaded; }

	};

}

