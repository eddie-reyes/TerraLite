#pragma once

#include "imgui.h"

namespace UI {

	class UISidebar {


	public:

		UISidebar();
		~UISidebar();
		void Draw();

	private:

		ImGuiWindowFlags m_windowFlags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBringToFrontOnFocus;

	};


}