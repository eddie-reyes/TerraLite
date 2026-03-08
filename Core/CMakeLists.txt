set(SOURCES "Main.cpp" "Application.h" "Application.cpp"  "Vendor/stb_image.cpp" "Vendor/stb_image.cpp" "Renderer/Renderer.h"  "Renderer/VertexBuffer.h"  "Renderer/IndexBuffer.h" "Renderer/Renderer.cpp" "Renderer/VertexArray.h" "Renderer/TerrainGeometry.h" "Renderer/TerrainGeometry.cpp" "Renderer/VertexBufferLayout.h" "Renderer/Shader.h" "Renderer/Shader.cpp" "Utils/Maths.h" "Renderer/GLUtils.h" "Renderer/GLUtils.cpp" "Event.h" "InputEvents.h" "UI/UIManager.h" "UI/UIManager.cpp" "UI/UISidebar.h" "UI/UISidebar.cpp" "UI/UIMenuBar.cpp" "UI/UIMenuBar.h")

# Add source to this project's executable.
add_executable (OpenGLFun)

target_sources(OpenGLFun PRIVATE ${SOURCES})

target_link_libraries(OpenGLFun PRIVATE glfw glad glm imgui)

target_include_directories(OpenGLFun PRIVATE Utils Vendor UI)

add_custom_command(
    TARGET OpenGLFun POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_CURRENT_SOURCE_DIR}/Renderer/Shaders 
            ${CMAKE_CURRENT_BINARY_DIR}/Renderer/Shaders 
    COMMENT "Copying shader files to build directory"
)