#include "UIManager.h"
#include <iostream>

#include "../Application.h"

namespace UI {


	void UIManager::Init() {

		GLFWwindow* window = Application::Get().GetWindowHandle();

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; 
		//io.FontGlobalScale = 1.5f;
		bool GLFWresult = ImGui_ImplGlfw_InitForOpenGL(window, true);

		if (!GLFWresult) {
			std::cerr << "Failed to initialize ImGui GLFW backend. Cannot proceed!" << std::endl;
		}

		bool OpenGLresult = ImGui_ImplOpenGL3_Init();

		if (!OpenGLresult) {
			std::cerr << "Failed to initialize ImGui OpenGL backend. Cannot proceed!" << std::endl;
		}

		m_UIHovered = &io.WantCaptureMouse;

	}

	void UIManager::Draw(float dt) {

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		m_MenuBar.Draw();
		m_Sidebar.Draw();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	}

	UIManager::~UIManager() {

		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();

	}

};