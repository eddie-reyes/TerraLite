#include "UIMenuBar.h"
#include "../Application.h"


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
                if (ImGui::MenuItem("Create")) { /* Do something */ }
                if (ImGui::MenuItem("Open", "Ctrl+O")) { /* Do something */ } // Add a shortcut label
                if (ImGui::MenuItem("Save", "Ctrl+S")) { /* Do something */ }
                ImGui::EndMenu(); // Ends the "File" menu
            }
            if (ImGui::BeginMenu("Edit"))
            {
                if (ImGui::MenuItem("Undo")) { /* Do something */ }
                ImGui::EndMenu(); // Ends the "Edit" menu
            }
            ImGui::EndMainMenuBar(); // Ends the main menu bar
        }

	}

}