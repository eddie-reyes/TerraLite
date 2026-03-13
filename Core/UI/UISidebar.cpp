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
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 10.0f));

		auto& exposedVars = Renderer::TerrainGeometry::GetExposedVars();
		auto& renderer = Renderer::Renderer::Get();

		if (ImGui::Begin("##sidebar", nullptr, m_windowFlags))
		{

			ImGui::Spacing();

			ImGui::Text("Z Scale");
			ImGui::SliderFloat("##z_scale", &exposedVars.ZScale, 0.01f, 1.0f, "%.3f", m_sliderFlags);
			if (ImGui::IsItemDeactivatedAfterEdit()) renderer.RebuildGeometryAndUpdateBuffers();
			ImGui::Text("Noise Scale");
			ImGui::SliderFloat("##noise_scale", &exposedVars.PerlinNoiseScale, 0.01f, 5.0f, "%.3f", m_sliderFlags);
			if (ImGui::IsItemDeactivatedAfterEdit()) renderer.RebuildGeometryAndUpdateBuffers();	
			ImGui::Text("Noise Offset X");
			ImGui::SliderFloat("##noise_offset_x", &exposedVars.PerlinNoiseOffset.x, -5.0f, 5.0f, "%.3f", m_sliderFlags);
			if (ImGui::IsItemDeactivatedAfterEdit()) renderer.RebuildGeometryAndUpdateBuffers();	
			ImGui::Text("Noise Offset Y");
			ImGui::SliderFloat("##noise_offset_y", &exposedVars.PerlinNoiseOffset.y, -5.0f, 5.0f, "%.3f", m_sliderFlags);
			if (ImGui::IsItemDeactivatedAfterEdit()) renderer.RebuildGeometryAndUpdateBuffers();	

			ImGui::Spacing();
			ImGui::Separator();
			ImGui::Spacing();

			ImGui::Text("Perlin Mix");
			ImGui::SliderFloat("##perlin_mix", &exposedVars.PerlinMix, 0.0f, 1.0f, "%.3f", m_sliderFlags);
			if (ImGui::IsItemDeactivatedAfterEdit()) renderer.RebuildGeometryAndUpdateBuffers();
			ImGui::Text("Square Diamond Mix");
			ImGui::SliderFloat("##square_diamond_mix", &exposedVars.DiamondSquareMix, 0.0f, 1.0f, "%.3f", m_sliderFlags);
			if (ImGui::IsItemDeactivatedAfterEdit()) renderer.RebuildGeometryAndUpdateBuffers();
			ImGui::Text("Voronoi Mix");
			ImGui::SliderFloat("##voronoi_mix", &exposedVars.VoronoiMix, 0.0f, 1.0f, "%.3f", m_sliderFlags);
			if (ImGui::IsItemDeactivatedAfterEdit()) renderer.RebuildGeometryAndUpdateBuffers();

			ImGui::Checkbox("Perturb Mix", &exposedVars.PerturbEnabled);
			if (ImGui::IsItemClicked()) renderer.RebuildGeometryAndUpdateBuffers(true);

			
			ImGui::Spacing();
			ImGui::Separator();
			ImGui::Spacing();
			
			ImGui::Text("Resolution");
			if (ImGui::Combo("##resolution", &m_selectedResolutionIdx, Globals::SupportedResolutions, IM_ARRAYSIZE(Globals::SupportedResolutions)))
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
		ImGui::PopStyleVar();

	}

}