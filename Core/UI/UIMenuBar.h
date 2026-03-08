#pragma once

#include "imgui.h"

namespace UI {

	class UIMenuBar {


	public:

		UIMenuBar();
		~UIMenuBar();
		void Draw();

	private:

		ImGuiWindowFlags m_windowFlags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse;

		float m_menubarHeight = 20.0f;

	};


}