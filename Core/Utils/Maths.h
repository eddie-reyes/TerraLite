#include <iostream>


namespace Utils {


    inline float normalize_value(float value, float current_min, float current_max, float target_min, float target_max) {
        if (current_max == current_min) return target_min; // Avoid division by zero

        float normalized = target_min + ((value - current_min) * (target_max - target_min)) / (current_max - current_min);
        return normalized;
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