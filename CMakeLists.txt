cmake_minimum_required (VERSION 3.11)
project ("OpenGLFun")

set(CMAKE_CXX_STANDARD 23)

# Generate compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(Dependencies.cmake)

add_subdirectory(Core)

add_compile_definitions(
    GLFW_INCLUDE_NONE
)