#pragma once

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#include "UISidebar.h"
#include "UIMenuBar.h"

namespace UI {

	class UIManager
	{
	public:

		void Init();
		void Draw();

		bool isUIHovered() const { return *m_UIHovered; };
		void Shutdown();

	private:

		UISidebar m_Sidebar;

		UIMenuBar m_MenuBar;

		bool * m_UIHovered;

	};

}
