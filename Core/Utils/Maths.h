#include <random>
#include <iomanip>


namespace Utils {


	inline float randomFloatUniform(float min, float max) {

		static std::random_device rd;
		static std::mt19937 engine(rd());
		std::uniform_real_distribution<float> dist(min, max);

		return dist(engine);

	}

    inline float NormalizeValueRange(float value, float current_min, float current_max, float target_min, float target_max) {
        if (current_max == current_min) return target_min; // Avoid division by zero

        float normalized = target_min + ((value - current_min) * (target_max - target_min)) / (current_max - current_min);
        return normalized;
    }

	inline float DistApprox(float x1, float y1, float x2, float y2, size_t size)
	{
		float dx = std::abs(x1 - x2);
		float dy = std::abs(y1 - y2);

		//choose minimum of wrapped distance vs. physical distance (Torodial distance)
		dx = std::min(dx, size - dx);
		dy = std::min(dy, size - dy);   

		return dx * dx + dy * dy; // fast distance formula (adapted from Realtime Procedural Terrain Generation paper)
	}

	inline float Lerp(float a, float b, float t) {

		return a + t * (b - a);

	}

	inline void OrbitAroundCenter(glm::mat4& View, glm::mat4& Model, float mouseDeltaX, float mouseDeltaY)
	{

		glm::vec3 viewMatrix = glm::normalize(glm::vec3(View * glm::vec4(0, 0, 1, 0)));

		View = glm::rotate(View, glm::radians(viewMatrix.y >= 0.0 ? 0.01f : (float)mouseDeltaY), glm::vec3(1, 0, 0));

		glm::vec3 rotationMatrix = glm::normalize(glm::vec3(Model * glm::vec4(0, 0, 1, 0)));
		glm::vec3 normalModel = glm::cross(rotationMatrix, glm::vec3(1, 0, 0));

		Model = glm::rotate(Model, glm::radians((float)-mouseDeltaX), normalModel);

	}

	inline glm::vec2 GetMouseDelta(float mouseX, float mouseY) {

		static float currMouseX = mouseX;
		static float currMouseY = mouseY;
		float deltaX = mouseX - currMouseX;
		float deltaY = mouseY - currMouseY;
		
		return glm::vec2(deltaX, deltaY);
	}

}