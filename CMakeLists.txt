# CMakeList.txt : CMake project for OpenGLFun, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.11)
project ("OpenGLFun")

set(CMAKE_CXX_STANDARD 23)

# Generate compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(Dependencies.cmake)

# Add source to this project's executable.
add_executable (OpenGLFun "OpenGLFun.cpp" )

add_compile_definitions(
    GLFW_INCLUDE_NONE
)

target_link_libraries(OpenGLFun PRIVATE glfw glad glm imgui)

add_custom_command(
    TARGET OpenGLFun POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_CURRENT_SOURCE_DIR}/shaders # Source directory
            ${CMAKE_CURRENT_BINARY_DIR}/shaders # Destination directory
    COMMENT "Copying shader files to build directory"
)

# Set working directory for VS debugger
set_target_properties(OpenGLFun PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/OpenGLFun"
)