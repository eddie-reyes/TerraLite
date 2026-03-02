set(SOURCES "Main.cpp" "Renderer.cpp")

# Add source to this project's executable.
add_executable (OpenGLFun)

target_sources(OpenGLFun PRIVATE ${SOURCES})

target_link_libraries(OpenGLFun PRIVATE glfw glad glm imgui)

target_include_directories(OpenGLFun PRIVATE Math)

add_custom_command(
    TARGET OpenGLFun POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_CURRENT_SOURCE_DIR}/shaders 
            ${CMAKE_CURRENT_BINARY_DIR}/shaders 
    COMMENT "Copying shader files to build directory"
)