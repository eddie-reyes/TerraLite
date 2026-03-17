#pragma once

#include "imgui.h"



namespace UI {

	class UISidebar {


	public:

		UISidebar();
		~UISidebar();
		void Draw();

		void NewSection(const char* title, bool topSection = false);


	private:

		ImGuiWindowFlags m_windowFlags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBringToFrontOnFocus;

		ImGuiSliderFlags m_sliderFlags = ImGuiSliderFlags_AlwaysClamp | ImGuiSliderFlags_NoRoundToFormat;

		int m_selectedResolutionIdx = 3;

	};


}