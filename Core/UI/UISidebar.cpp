#include "UISidebar.h".
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

		auto& exposedVars = Renderer::TerrainGeometry::GetExposedVars();
		auto& renderer = Renderer::Renderer::Get();

		if (ImGui::Begin("Sidebar", nullptr, m_windowFlags))
		{

			ImGui::SliderFloat("Z Scale", &exposedVars.ZScale, 0.01f, 1.0f, "%.3f", m_sliderFlags);

			if (ImGui::IsItemDeactivatedAfterEdit()) renderer.RebuildGeometryAndUpdateBuffers(false);

			ImGui::SliderFloat("Noise Scale", &exposedVars.NoiseScale, 0.01f, 5.0f, "%.3f", m_sliderFlags);

			if (ImGui::IsItemDeactivatedAfterEdit()) renderer.RebuildGeometryAndUpdateBuffers(false);	

			const char* items[] = { "64x64", "128x128", "256x256", "512x512", "1024x1024"};
			if (ImGui::Combo("Resolution", &m_selectedResolutionIdx, items, IM_ARRAYSIZE(items)))
			{

				switch (m_selectedResolutionIdx) {
				case 0: exposedVars.Resolution = 64; break;
				case 1: exposedVars.Resolution = 128; break;
				case 2: exposedVars.Resolution = 256; break;
				case 3: exposedVars.Resolution = 512; break;
				case 4: exposedVars.Resolution = 1024; break;
				}

				renderer.RebuildGeometryAndUpdateBuffers(true);
			}
			
		}
		ImGui::End();
		ImGui::PopStyleColor();

	}

}