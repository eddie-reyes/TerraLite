#pragma once

#include "UIMenuBar.h"
#include "../Application.h"
#include "../Utils/FileIO.h"


namespace UI {


	void UIMenuBar::Draw() {

        if (ImGui::BeginMainMenuBar())
        {

            if (ImGui::BeginMenu("File"))
            {
                auto& exposedVars = Renderer::TerrainGeometry::GetExposedVars();
                auto& renderer = Renderer::Renderer::Get();

                if (ImGui::MenuItem("Open", "Ctrl+O")) { 
                
                    if (FileIO::OpenHeightMap(*exposedVars.TerrainVertices)) {

                        renderer.RebuildGeometryAndUpdateBuffers(false, false);

                    }
                
                } 
                if (ImGui::MenuItem("Save", "Ctrl+S")) {
                
                    FileIO::SaveHeightMapToFile(*exposedVars.TerrainVertices, exposedVars.Resolution);
                
                }
                ImGui::EndMenu(); 
            }
            ImGui::EndMainMenuBar(); 
        }

	}

}