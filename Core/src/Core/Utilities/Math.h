#pragma once
#include <glm/glm.hpp>

namespace Core {

	namespace Math {
		inline float Lerp(float a, float b, float t) {
			return a + (b - a) * t;
		}
		inline glm::vec2 Lerp(const glm::vec2& a, const glm::vec2& b, float t) {
			return {
				Lerp(a.x, b.x, t),
				Lerp(a.y, b.y, t)
			};
		}
		inline glm::vec3 Lerp(const glm::vec3& a, const glm::vec3& b, float t) {
			return {
				Lerp(a.x, b.x, t),
				Lerp(a.y, b.y, t),
				Lerp(a.z, b.z, t)
			};
		}
		inline glm::vec4 Lerp(const glm::vec4& a, const glm::vec4& b, float t) {
			return {
				Lerp(a.x, b.x, t),
				Lerp(a.y, b.y, t),
				Lerp(a.z, b.z, t),
				Lerp(a.w, b.w, t)
			};
		}
	}

}
