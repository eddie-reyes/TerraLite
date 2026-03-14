set(SOURCES "Main.cpp" "Application.h" "Application.cpp" "Renderer/Renderer.h"  "Renderer/VertexBuffer.h"  "Renderer/IndexBuffer.h" "Renderer/Renderer.cpp" "Renderer/VertexArray.h" "Renderer/TerrainGeometry.h" "Renderer/TerrainGeometry.cpp" "Renderer/VertexBufferLayout.h" "Renderer/Shader.h" "Renderer/Shader.cpp" "Utils/Maths.h" "Renderer/GLUtils.h" "Renderer/GLUtils.cpp" "Event.h" "InputEvents.h" "UI/UIManager.h" "UI/UIManager.cpp" "UI/UISidebar.h" "UI/UISidebar.cpp" "UI/UIMenuBar.cpp" "UI/UIMenuBar.h" "Utils/Globals.h" "Utils/Noise.h" "WindowEvents.h" "Utils/FileIO.h")

# Add source to this project's executable.
add_executable (TerraLite)

target_sources(TerraLite PRIVATE ${SOURCES})

target_link_libraries(TerraLite PRIVATE glfw glad glm imgui)

target_include_directories(TerraLite PRIVATE Utils Vendor UI Renderer Renderer/Shaders)

add_custom_command(
    TARGET TerraLite POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_CURRENT_SOURCE_DIR}/Renderer/Shaders 
            ${CMAKE_CURRENT_BINARY_DIR}/Renderer/Shaders 
    COMMENT "Copying shader files to build directory"
)