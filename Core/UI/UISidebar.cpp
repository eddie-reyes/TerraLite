#include "UISidebar.h".
#include "../Utils/Globals.h"
#include "../Application.h"
#include <numbers>

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
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(15.0f, 15.0f));

		auto& exposedVars = Renderer::TerrainGeometry::GetExposedVars();
		auto& renderer = Renderer::Renderer::Get();

		if (ImGui::Begin("##sidebar", nullptr, m_windowFlags))
		{

			NewSection("Transform", true);

			ImGui::Text("Z Scale");
			ImGui::SliderFloat("##z_scale", &exposedVars.ZScale, 0.01f, 1.0f, "%.3f", m_sliderFlags);
			if (ImGui::IsItemDeactivatedAfterEdit()) renderer.RebuildGeometryAndUpdateBuffers();
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

			NewSection("Diamond Square");

			ImGui::Text("Amplitude");
			ImGui::SliderFloat("##square_diamond_amplitude", &exposedVars.DiamondSquareAmplitude, 0.1f, 0.5f, "%.3f", m_sliderFlags);
			if (ImGui::IsItemDeactivatedAfterEdit()) renderer.RebuildGeometryAndUpdateBuffers();
			ImGui::Text("Persistance");
			ImGui::SliderFloat("##square_diamond_persistance", &exposedVars.DiamondSquarePersistance, 0.1f, 0.5f, "%.3f", m_sliderFlags);
			if (ImGui::IsItemDeactivatedAfterEdit()) renderer.RebuildGeometryAndUpdateBuffers();
			ImGui::Text("Mix");
			ImGui::SliderFloat("##square_diamond_mix", &exposedVars.DiamondSquareMix, 0.0f, 1.0f, "%.3f", m_sliderFlags);
			if (ImGui::IsItemDeactivatedAfterEdit()) renderer.RebuildGeometryAndUpdateBuffers();
			if (ImGui::Checkbox("Enabled##1", &exposedVars.DiamondSquareEnabled)) renderer.RebuildGeometryAndUpdateBuffers();

			NewSection("Voronoi");

			ImGui::Text("Cells Per Axis");
			ImGui::SliderInt("##voronoi_cpa", &exposedVars.VoronoiCellsPerAxis, 2, 10, "%d", m_sliderFlags);
			if (ImGui::IsItemDeactivatedAfterEdit()) renderer.RebuildGeometryAndUpdateBuffers();
			ImGui::Text("Feature Points");
			ImGui::SliderInt("##voronoi_ppc", &exposedVars.VoronoiFeaturePointsPerCell, 1, 10, "%d", m_sliderFlags);
			if (ImGui::IsItemDeactivatedAfterEdit()) renderer.RebuildGeometryAndUpdateBuffers();
			ImGui::Text("Mix");
			ImGui::SliderFloat("##voronoi_mix", &exposedVars.VoronoiMix, 0.0f, 1.0f, "%.3f", m_sliderFlags);
			if (ImGui::IsItemDeactivatedAfterEdit()) renderer.RebuildGeometryAndUpdateBuffers();
			if (ImGui::Checkbox("Enabled##2", &exposedVars.VoronoiEnabled)) renderer.RebuildGeometryAndUpdateBuffers();

			NewSection("Perturbation");

			ImGui::Text("Max Displacement");
			ImGui::SliderFloat("##perturb_max_displacement", &exposedVars.PerturbMaxDisplacement, 0.1, 1, "%.3f", m_sliderFlags);
			if (ImGui::IsItemDeactivatedAfterEdit()) renderer.RebuildGeometryAndUpdateBuffers();
			if (ImGui::Checkbox("Enabled##3", &exposedVars.PerturbEnabled)) renderer.RebuildGeometryAndUpdateBuffers();

			NewSection("Erosion");

			ImGui::Text("Iterations");
			ImGui::SliderInt("##erosion_iterations", &exposedVars.ErosionIterations, 1, 100, "%d", m_sliderFlags);
			if (ImGui::IsItemDeactivatedAfterEdit()) renderer.RebuildGeometryAndUpdateBuffers();
			ImGui::Text("Erosion Strength");
			ImGui::SliderFloat("##erosion_strength", &exposedVars.ErosionStrength, 0.1, 1, "%.3f", m_sliderFlags);
			if (ImGui::IsItemDeactivatedAfterEdit()) renderer.RebuildGeometryAndUpdateBuffers();
			ImGui::Text("Talus (Angle of Inclination)");
			ImGui::SliderFloat("##erosion_talus", &exposedVars.ErosionTalus, 0.0, 16.0, "%.3f", m_sliderFlags);
			if (ImGui::IsItemDeactivatedAfterEdit()) renderer.RebuildGeometryAndUpdateBuffers();
			if (ImGui::Checkbox("Enabled##4", &exposedVars.ErosionEnabled)) renderer.RebuildGeometryAndUpdateBuffers();
			
		}

		ImGui::End();
		ImGui::PopStyleColor();
		ImGui::PopStyleVar();
		ImGui::PopStyleVar();

	}

	void UISidebar::NewSection(const char* title, bool topSection)
	{

		if (!topSection) {
			ImGui::Spacing();
			ImGui::Separator();
		}

		float windowWidth = ImGui::GetWindowContentRegionMax().x - ImGui::GetWindowContentRegionMin().x;
		float textWidth = ImGui::CalcTextSize(title).x;
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (windowWidth - textWidth) * 0.5f);
		ImGui::Text(title);

		ImGui::Spacing();
	}

}