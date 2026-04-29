cmake_minimum_required(VERSION 3.17)
project(ILikeMyIceCrushed VERSION 0.6.7 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
add_compile_options(-Wall -Wextra -Wpedantic)

set(IMGUI_SOURCES
    src/imgui/imgui.cpp
    src/imgui/imgui_demo.cpp
    src/imgui/imgui_draw.cpp
    src/imgui/imgui_tables.cpp
    src/imgui/imgui_widgets.cpp
    src/imgui/imgui_impl_glfw.cpp
    src/imgui/imgui_impl_opengl3.cpp
)

set(DRIVER
    src/main.cpp
    ${IMGUI_SOURCES}
)

add_executable(appgui ${DRIVER})

target_include_directories(appgui PRIVATE src/gui/imgui)