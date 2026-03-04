#include <glm/gtc/noise.hpp>

namespace Core::Utils {
	static inline float PerlinNoise2D(float x, float y) {
		return glm::perlin(glm::vec2(x, y));
	}
}