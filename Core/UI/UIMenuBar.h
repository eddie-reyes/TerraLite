#pragma once

#include "imgui.h"

namespace UI {

	class UIMenuBar {


	public:

		void Draw();

	private:

		ImGuiWindowFlags m_windowFlags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse;

	};


}