#include "Renderer.h"
#include "Shader.h"
#include "../Application.h"
#include "../Utils/Maths.h"
#include "../Utils/Globals.h"


namespace Renderer {


	void Renderer::Init()
	{
		m_Shader = CreateGraphicsShader("Renderer/Shaders/transform.vert.glsl", "Renderer/Shaders/texture.frag.glsl");
		glUseProgram(m_Shader);

		GLFWwindow* window = Application::Get().GetWindowHandle();

		SetScene(window);

		m_Uniforms.push_back(UniformInfo{ "u_Color", glGetUniformLocation(m_Shader, "u_Color"), UniformType::VEC4F_UNIFORM });
		m_Uniforms.push_back(UniformInfo{ "MVP", glGetUniformLocation(m_Shader, "MVP"), UniformType::MVP_UNIFORM });

		m_TerrainGeometry.BuildPlane();
	
		std::vector<float>& vertices = m_TerrainGeometry.GetVertices();
		std::vector<unsigned int>& indices = m_TerrainGeometry.GetIndices();

		m_GeometryBuffers.emplace_back(std::make_unique<GeometryBufferData>(vertices.data(), vertices.size(), indices.data(), indices.size()));

		glfwSetMouseButtonCallback(window, [](GLFWwindow * window, int button, int action, int mods)
		{

			Application& app = Application::Get();

			switch (action)
			{
				case GLFW_PRESS:
				{
						
					MouseButtonPressedEvent event(button);
					app.RaiseEvent(event);
					break;
				}
				case GLFW_RELEASE:
				{
					MouseButtonReleasedEvent event(button);
					app.RaiseEvent(event);
					break;
				}
			}

		});

		glfwSetWindowSizeCallback(window, [](GLFWwindow* handle, int width, int height)
		{
			Application& app = Application::Get();

			WindowResizeEvent event((uint32_t)width, (uint32_t)height);
			app.RaiseEvent(event);
		});

		glfwSetScrollCallback(window, [](GLFWwindow* handle, double xOffset, double yOffset)
		{
			Application& app = Application::Get();

			MouseScrolledEvent event(xOffset, yOffset);
			app.RaiseEvent(event);
		});
	}

	void Renderer::SetScene(GLFWwindow* handle)
	{

		int width, height;
		glfwGetWindowSize(handle, &width, &height);

		glViewport(0, 0, width - Globals::SIDBAR_OFFSET, height);

		float aspectRatio = (float)width / (float)height;

		m_MVPMatrix.Projection = glm::perspective(glm::radians(m_FOV), aspectRatio, 0.1f, 100.0f);
		

		m_MVPMatrix.View = glm::lookAt(
			m_CameraPos,
			glm::vec3(0, 0, 0),
			glm::vec3(0, 1, 0)
		);

		//initial transformation for the plane
		m_MVPMatrix.Model = glm::mat4(1.0f);
		m_MVPMatrix.Model = glm::rotate(m_MVPMatrix.Model, glm::radians(90.0f), glm::vec3(1, 0, 0));
		m_MVPMatrix.Model = glm::rotate(m_MVPMatrix.Model, glm::radians(45.0f), glm::vec3(0, 0, 1));

	}

	void Renderer::Draw(float dt) {

		glClearColor(BG_COLOR.r, BG_COLOR.g, BG_COLOR.b, BG_COLOR.a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		AttachUniforms();

		if (m_isOrbitEnabled) {

			double mouseX, mouseY;
			GLFWwindow* window = Application::Get().GetWindowHandle();
			glfwGetCursorPos(window, &mouseX, &mouseY);
			glm::vec2 delta = Utils::GetMouseDelta((float)mouseX, (float)mouseY);

			Utils::OrbitAroundCenter(m_MVPMatrix.View, m_MVPMatrix.Model, delta.x * dt * ORBIT_SPEED, delta.y * dt * ORBIT_SPEED);
;		}

		glDrawElements(GL_TRIANGLES, m_TerrainGeometry.GetTriangleCount() * 6, GL_UNSIGNED_INT, nullptr);

	}

	void Renderer::OnEvent(Event& event)
	{

		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<MouseButtonPressedEvent>([this](MouseButtonPressedEvent& e) { return OnMouseButtonPressed(e); });
		dispatcher.Dispatch<MouseButtonReleasedEvent>([this](MouseButtonReleasedEvent& e) { return OnMouseButtonReleased(e); });
		dispatcher.Dispatch<WindowResizeEvent>([this](WindowResizeEvent& e) { return OnWindowResize(e); });
		dispatcher.Dispatch<MouseScrolledEvent>([this](MouseScrolledEvent& e) { return OnMouseScrolled(e); });

	}

	bool Renderer::OnMouseButtonPressed(MouseButtonPressedEvent& event)
	{

		if (event.GetMouseButton() == GLFW_MOUSE_BUTTON_MIDDLE || event.GetMouseButton() == GLFW_MOUSE_BUTTON_LEFT) {
			ToggleOrbit();
			return true;
		}

		return false;
	
	}

	bool Renderer::OnMouseButtonReleased(MouseButtonReleasedEvent& event)
	{

		if (event.GetMouseButton() == GLFW_MOUSE_BUTTON_MIDDLE || event.GetMouseButton() == GLFW_MOUSE_BUTTON_LEFT) {
			ToggleOrbit();
			return true;
		}

		return false;
	
	}

	bool Renderer::OnWindowResize(WindowResizeEvent& event)
	{
		
		glViewport(0, 0, event.GetWidth() - Globals::SIDBAR_OFFSET, event.GetHeight());

		return true;
	}

	bool Renderer::OnMouseScrolled(MouseScrolledEvent& event)
	{

		int width, height;
		int zoomDir = event.GetYOffset();
		glfwGetFramebufferSize(Application::Get().GetWindowHandle(), &width, &height);
		m_MVPMatrix.Projection = glm::perspective(glm::radians(m_FOV -= zoomDir), (float)width/height, 0.1f, 100.0f);
		m_FOV = glm::clamp(m_FOV, FOV_RANGE.x, FOV_RANGE.y);

		return true;
	}

	void Renderer::AttachUniforms() const
	{

		for (const auto& uniform : m_Uniforms) {
			switch (uniform.Type) {
			case UniformType::VEC4F_UNIFORM:
				glUniform4f(uniform.Location, DEFAULT_OPAQUE_COLOR.r, DEFAULT_OPAQUE_COLOR.g, DEFAULT_OPAQUE_COLOR.b, DEFAULT_OPAQUE_COLOR.a); 
				break;
			case UniformType::MVP_UNIFORM:
				glm::mat4 mvp = m_MVPMatrix.Compute();
				glUniformMatrix4fv(uniform.Location, 1, GL_FALSE, &mvp[0][0]);
				break;
			}
		}

	}

	Renderer::~Renderer()
	{
		glDeleteProgram(m_Shader);

	}
}

