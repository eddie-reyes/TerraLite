#include "UISidebar.h"
#include "../Application.h"


namespace UI {


	UISidebar::UISidebar() {

		

	}

	UISidebar::~UISidebar() {



	}


	void UISidebar::Draw() {

		int width, height;
		glfwGetFramebufferSize(Application::Get().GetWindowHandle(), &width, &height);
	
		ImGui::SetNextWindowPos(ImVec2((float)width - m_sidebarWidth, 0.0f), ImGuiCond_Always);
		ImGui::SetNextWindowSize(ImVec2(m_sidebarWidth, height), ImGuiCond_Always);

		if (ImGui::Begin("Generate Terrain", nullptr, m_windowFlags))
		{
			
		}
		ImGui::End();
		

	}

}