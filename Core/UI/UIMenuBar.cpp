#pragma once

#include "UIMenuBar.h"
#include "../Application.h"
#include "../Utils/FileIO.h"
#include <vector>


namespace UI {

    
	UIMenuBar::UIMenuBar() {

		

	}

	UIMenuBar::~UIMenuBar() {



	}


	void UIMenuBar::Draw() {

        if (ImGui::BeginMainMenuBar())
        {

            if (ImGui::BeginMenu("File"))
            {
                auto& exposedVars = Renderer::TerrainGeometry::GetExposedVars();

                if (ImGui::MenuItem("Create")) { /* Do something */ }
                if (ImGui::MenuItem("Open", "Ctrl+O")) { /* Do something */ } 
                if (ImGui::MenuItem("Save", "Ctrl+S")) {
                
                    FileIO::SaveHeightMapToFile(*exposedVars.TerrainVertices, exposedVars.Resolution,  1 / exposedVars.ZScale);
                
                }
                ImGui::EndMenu(); 
            }
            ImGui::EndMainMenuBar(); 
        }

	}

}