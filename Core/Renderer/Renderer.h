#pragma once

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "TerrainGeometry.h"

namespace Renderer {

	enum class UniformType {

		MVP_UNIFORM,
		VEC4F_UNIFORM

	};

	struct UniformInfo
	{
		const char* Name;
		int Location;
		UniformType Type;
	};

	struct MVPMatrix
	{
		glm::mat4 Model;
		glm::mat4 View;
		glm::mat4 Projection;

		inline glm::mat4 Compute() const {

			return Projection * View * Model;

		}
	};

	class Renderer
	{
	public:

		Renderer();
		~Renderer();

		void Init();
		void Draw(float dt);

		glm::vec3 GetCameraPosition() const { return m_CameraPos; }

		void AttachUniforms() const ;


	private:

		TerrainGeometry m_TerrainGeometry;

		float m_distanceFromOrgin = 2.0f;
		
		glm::vec3 m_CameraPos = { m_distanceFromOrgin, m_distanceFromOrgin, m_distanceFromOrgin };

		uint32_t m_Shader;

		MVPMatrix m_MVPMatrix;

		std::vector<UniformInfo> m_Uniforms;
		

	};

};
