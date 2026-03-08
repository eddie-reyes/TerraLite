#include "UISidebar.h"
#include "../Application.h"
#include "../Utils/Globals.h"


namespace UI {


	UISidebar::UISidebar() {

		

	}

	UISidebar::~UISidebar() {



	}


	void UISidebar::Draw() {

		int width, height;
		glfwGetFramebufferSize(Application::Get().GetWindowHandle(), &width, &height);
	
		ImGui::SetNextWindowPos(ImVec2((float)width - Globals::SIDBAR_OFFSET, 0.0f), ImGuiCond_Always);
		ImGui::SetNextWindowSize(ImVec2(Globals::SIDBAR_OFFSET, height), ImGuiCond_Always);
		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.15f, 0.15f, 0.15f, 1.0f));

		if (ImGui::Begin("Sidebar", nullptr, m_windowFlags))
		{
			
		}
		ImGui::End();
		ImGui::PopStyleColor();

	}

}