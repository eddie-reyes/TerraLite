#pragma once

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "TerrainGeometry.h" 
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "../InputEvents.h"
#include "../WindowEvents.h"

#include <memory>

constexpr float ORBIT_SPEED = 0.3;
constexpr glm::vec4 BG_COLOR = { 0.0f, 0.2f, 0.4f, 1.0f };
constexpr glm::vec4 DEFAULT_OPAQUE_COLOR = { 0.8f, 0.8f, 0.8f, 1.0f };
constexpr glm::vec2 FOV_RANGE = { 15.0f, 70.0f };

namespace Renderer {

	enum class UniformType {

		VIEW_UNIFORM,
		MODEL_UNIFORM,
		PROJECTION_UNIFORM,
		MAT_COLOR_UNIFORM,

	};

	struct UniformInfo
	{
		const char* Name;
		int Location;
		UniformType Type;
	};

	struct MVPMatrix
	{
		inline glm::mat4 Compute() const { return Projection * View * Model; }

		glm::mat4 Model;
		glm::mat4 View;
		glm::mat4 Projection;

	};

	struct GeometryBufferData {

		GeometryBufferData(float * verticesData, size_t vertexCount, float * normalData, unsigned int * indicesData, size_t indexCount) {
		
			layout.Push<float>(3);

			va[0].Bind();

			//vertex buffer for positions w/ indices
			vb[0].CreateBuffer(verticesData, vertexCount);
			ib.CreateBuffer(indicesData, indexCount);
			va[0].AddBuffer(layout);

			va[1].Bind();

			//vertex buffer for normals
			vb[1].CreateBuffer(normalData, vertexCount);
			va[1].AddBuffer(layout);

			va[0].Bind();

		}

		VertexArray va[2];
		VertexBufferLayout layout;
		VertexBuffer vb[2];
		IndexBuffer ib;

	};

	class Renderer
	{
	public:

		~Renderer();

		void Init();
		void Draw(float dt);
		void OnEvent(Event& event);

		void SetScene(GLFWwindow* handle);

		glm::vec3 GetCameraPosition() const { return m_CameraPos; }

		void AttachUniforms() const ;

		void ToggleOrbit() { m_isOrbitEnabled = !m_isOrbitEnabled; };

		bool OnMouseButtonPressed(MouseButtonPressedEvent& event);
		bool OnMouseButtonReleased(MouseButtonReleasedEvent& event);
		bool OnWindowResize(WindowResizeEvent& event);
		bool OnMouseScrolled(MouseScrolledEvent& event);


	private:

		TerrainGeometry m_TerrainGeometry;
		
		glm::vec3 m_CameraPos = { 0, 2, 2 };
		float m_FOV = FOV_RANGE.y;

		uint32_t m_Shader;

		MVPMatrix m_MVPMatrix;

		std::vector<UniformInfo> m_Uniforms;

		std::vector<std::unique_ptr<GeometryBufferData>> m_GeometryBuffers;

		bool m_isOrbitEnabled = false;

	};

};
