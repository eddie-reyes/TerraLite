#include "Renderer.h"
#include "Shader.h"
#include "../Application.h"


namespace Renderer {

	Renderer::Renderer()
	{
		m_Shader = CreateGraphicsShader("Renderer/Shaders/transform.vert.glsl", "Renderer/Shaders/texture.frag.glsl");
		glUseProgram(m_Shader);

		int width, height;
		GLFWwindow* window = Application::Get().GetWindowHandle();
		glfwGetWindowSize(window, &width, &height);

		m_MVPMatrix.Projection = glm::perspective(glm::radians(90.0f), (float)width / (float)height, 0.1f, 100.0f);

		m_MVPMatrix.View = glm::lookAt(
			m_CameraPos,  
			glm::vec3(0, 0, 0), 
			glm::vec3(0, 1, 0)  
		);

		m_MVPMatrix.Model = glm::mat4(1.0f);
		m_MVPMatrix.Model = glm::rotate(m_MVPMatrix.Model, glm::radians(90.0f), glm::vec3(1, 0, 0));
		m_MVPMatrix.Model = glm::rotate(m_MVPMatrix.Model, glm::radians(45.0f), glm::vec3(0, 0, 1));

		m_Uniforms.push_back(UniformInfo{ "u_Color", glGetUniformLocation(m_Shader, "u_Color"), UniformType::VEC4F_UNIFORM });
		m_Uniforms.push_back(UniformInfo{ "MVP", glGetUniformLocation(m_Shader, "MVP"), UniformType::MVP_UNIFORM });

	}

	Renderer::~Renderer()
	{
		glDeleteProgram(m_Shader);

	}

	void Renderer::Init()
	{

		m_TerrainGeometry.BuildPlane();

		VertexArray va;
		va.Bind();

		VertexBuffer vb(m_TerrainGeometry.GetVertices().data(), m_TerrainGeometry.GetVertices().size() * sizeof(float));
		vb.Bind();

		VertexBufferLayout layout;
		layout.Push<float>(3);
		va.AddBuffer(vb, layout);

		IndexBuffer ib(m_TerrainGeometry.GetIndices().data(), m_TerrainGeometry.GetIndices().size());

		
	}

	void Renderer::Draw(float dt) {

		glClear(GL_COLOR_BUFFER_BIT);

		AttachUniforms();
		glDrawElements(GL_TRIANGLES, m_TerrainGeometry.GetTriangleCount(), GL_UNSIGNED_INT, nullptr);

	}

	void Renderer::AttachUniforms() const
	{

		for (const auto& uniform : m_Uniforms) {
			switch (uniform.Type) {
			case UniformType::VEC4F_UNIFORM:
				glUniform4f(uniform.Location, 1.0f, 0.0f, 0.0f, 1.0f); // Example: Set to red color
				break;
			case UniformType::MVP_UNIFORM:
				glm::mat4 mvp = m_MVPMatrix.Compute();
				glUniformMatrix4fv(uniform.Location, 1, GL_FALSE, &mvp[0][0]);
				break;
			}
		}

	}

}

