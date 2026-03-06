#pragma once

#include <string>

#include "Renderer/Renderer.h"



struct ApplicationSpecification
{
	std::string Name;
	unsigned int WindowWidth;
	unsigned int WindowHeight;
	unsigned int HeightMapResolution;
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
	ApplicationSpecification& GetAppSpecification() { return m_Specification; }

private:

	bool m_Running = false;

	ApplicationSpecification m_Specification;

	Renderer::Renderer m_Renderer;

	GLFWwindow* m_WindowHandle;

};