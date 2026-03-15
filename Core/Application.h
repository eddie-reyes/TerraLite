#pragma once

#include <string>

#include "Renderer/Renderer.h"
#include "UI/UIManager.h"
#include "Events/Event.h"

const unsigned int MIN_WINDOW_WIDTH = 1280;
const unsigned int MIN_WINDOW_HEIGHT = 720;

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
	void RaiseEvent(Event& event);


	static Application& Get();
	static float GetTime();
	GLFWwindow*  GetWindowHandle() { return m_WindowHandle; }
	ApplicationSpecification& GetSpecification() { return m_Specification; }

private:

	bool m_Running = false;

	ApplicationSpecification m_Specification;

	Renderer::Renderer m_Renderer;

	UI::UIManager m_UIManager;

	GLFWwindow* m_WindowHandle;

};