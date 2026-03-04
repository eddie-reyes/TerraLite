#pragma once

#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <string>


namespace Core {

	struct ApplicationSpecification
	{
		std::string Name;
		unsigned int WindowWidth;
		unsigned int WindowHeight;
	};

	class Application
	{
	public:
		Application(const ApplicationSpecification& specification);
		~Application();

		void Run();
		void Stop();


		static Application& Get();
		static float GetTime();

	private:

		bool m_Running = false;

		ApplicationSpecification m_Specification;

		GLFWwindow* m_WindowHandle;

	};

}