set(SOURCES "Main.cpp" "Application.h" "Application.cpp"  "Vendor/stb_image.cpp" "Vendor/stb_image.cpp" "Renderer/Renderer.h"  "Renderer/VertexBuffer.h"  "Renderer/IndexBuffer.h" "Renderer/Renderer.cpp" "Renderer/VertexArray.h" "Renderer/TerrainGeometry.h" "Renderer/TerrainGeometry.cpp" "Renderer/VertexBufferLayout.h")

# Add source to this project's executable.
add_executable (OpenGLFun)

target_sources(OpenGLFun PRIVATE ${SOURCES})

target_link_libraries(OpenGLFun PRIVATE glfw glad glm imgui)

target_include_directories(OpenGLFun PRIVATE Utils Vendor UI)

add_custom_command(
    TARGET OpenGLFun POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_CURRENT_SOURCE_DIR}/shaders 
            ${CMAKE_CURRENT_BINARY_DIR}/shaders 
    COMMENT "Copying shader files to build directory"
)