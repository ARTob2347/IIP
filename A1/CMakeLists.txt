cmake_minimum_required(VERSION 3.10)
project(A1)

set(CMAKE_CXX_STANDARD 17)

find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)

include_directories(
    components/declaration
    external/imgui
    external/imgui/backends
)

set(IMGUI_FILES
    "external/imgui/imgui.cpp"
    "external/imgui/imgui_demo.cpp"
    "external/imgui/imgui_draw.cpp"
    "external/imgui/imgui_tables.cpp"
    "external/imgui/imgui_widgets.cpp"
    "external/imgui/backends/imgui_impl_glfw.cpp"
    "external/imgui/backends/imgui_impl_opengl3.cpp"
)

set(PROJECT_FILES
    "main.cpp"
    "components/definition/code_generator.cpp"
    "components/definition/saves.cpp"
    "components/definition/commands.cpp"
)

add_executable(TEST ${PROJECT_FILES} ${IMGUI_FILES})

target_link_libraries(TEST glfw OpenGL::GL)