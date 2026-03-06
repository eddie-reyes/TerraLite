#include "Application.h"

#include <assert.h>
#include <iostream>



static Application* s_Application = nullptr;

static void GLFWErrorCallback(int error, const char* description)
{
	std::cerr << "[GLFW Error]: " << description << std::endl;
}

Application::Application(const ApplicationSpecification& specification)
	: m_Specification(specification)
{
	s_Application = this;

	glfwSetErrorCallback(GLFWErrorCallback);
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

	//core profile for modern opengl
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

	GLFWwindow* handle = glfwCreateWindow(specification.WindowWidth, specification.WindowHeight, specification.Name.c_str(), nullptr, nullptr);

	if (handle == GL_FALSE)
	{
		std::cerr << "Failed to create GLFW window, cannot proceed!\n";
		return;
	}
	m_WindowHandle = handle;

	std::cout << "GLFW v" << GLFW_VERSION_MAJOR << '.' << GLFW_VERSION_MINOR << " Initialized Window (" << specification.WindowWidth << " x " << specification.WindowHeight << ")\n";

	glfwMakeContextCurrent(handle);
	glfwSwapInterval(1);
	GLuint loadGLresult = gladLoadGL(glfwGetProcAddress);

	if (loadGLresult == GL_FALSE) {
		std::cerr << "Failed to initialize GLAD, cannot proceed!\n";
		return;
	}

	std::cout << "glad v" << GLAD_VERSION_MAJOR(loadGLresult) << '.' << GLAD_VERSION_MINOR(loadGLresult) << " Initialized\n";


	m_Renderer.Init();


}

Application::~Application()
{

	glfwTerminate();

	s_Application = nullptr;
}

void Application::Run()
{
	m_Running = true;

	float lastTime = GetTime();

	// Main Application loop
	while (m_Running)
	{
		glfwPollEvents();

		if (glfwWindowShouldClose(m_WindowHandle))
		{
			Stop();
			break;
		}

		float currentTime = GetTime();
		float dt = glm::clamp(currentTime - lastTime, 0.001f, 0.1f);
		lastTime = currentTime;

		m_Renderer.Draw(dt);

		glfwSwapBuffers(m_WindowHandle);

	}
}

void Application::Stop()
{
	m_Running = false;
}

Application& Application::Get()
{
	assert(s_Application);
	return *s_Application;
}

float Application::GetTime()
{
	return (float)glfwGetTime();
}

