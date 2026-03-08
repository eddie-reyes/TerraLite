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
		io.FontGlobalScale = 1.5f;
		ImGui_ImplGlfw_InitForOpenGL(window, true);           
		ImGui_ImplOpenGL3_Init();

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