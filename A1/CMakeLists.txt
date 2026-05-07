cmake_minimum_required(VERSION 3.10)
cmake_minimum_required(VERSION 3.10)
project(A1)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenGL REQUIRED)

add_executable(A1
    main.cpp
    components/definition/code_generator.cpp
    components/definition/saves.cpp
    components/definition/commands.cpp
    external/imgui/backends/imgui_stdlib.cpp
    external/imgui/imgui.cpp
    external/imgui/imgui_demo.cpp
    external/imgui/imgui_draw.cpp
    external/imgui/imgui_tables.cpp
    external/imgui/imgui_widgets.cpp
    external/imgui/backends/imgui_impl_glfw.cpp
    external/imgui/backends/imgui_impl_opengl3.cpp
 "components/declaration/commands.h")

 if(WIN32)
    target_link_options(A1 PRIVATE -mconsole)
endif()

target_include_directories(A1 PRIVATE
    components/declaration
    external/imgui
    external/imgui/backends
    external/glfw/include
)

target_link_libraries(A1 PRIVATE
    "${CMAKE_SOURCE_DIR}/external/glfw/lib-vc2022/glfw3.lib"
    OpenGL::GL
    gdi32
    user32
    shell32
)
