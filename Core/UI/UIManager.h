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

		~UIManager();

		void Init();
		void Draw(float dt);

	private:

		UISidebar m_Sidebar;

		UIMenuBar m_MenuBar;

	};

}
