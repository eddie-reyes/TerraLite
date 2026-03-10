#include <glm/gtc/noise.hpp>

namespace Utils {
	inline float PerlinNoise2D(float x, float y) {
		return glm::perlin(glm::vec2(x, y));
	}
}