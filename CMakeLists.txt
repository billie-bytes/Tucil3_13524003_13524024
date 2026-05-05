cmake_minimum_required(VERSION 3.10)


project(IceSlide VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_BUILD_TYPE Debug)

# DEPENDENCIES: OpenGL & GLFW
# 
# SYSTEM INSTALLATION INSTRUCTIONS (Debian/Ubuntu/WSL):
# If GLFW and OpenGL are not installed on the system, install them by running:
#   sudo apt update
#   sudo apt install libglfw3-dev libgl1-mesa-dev xorg-dev


find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)


set(CORE_SOURCES
    src/core/Board.cpp
    src/core/BoardAlg.cpp
)

set(IMGUI_SOURCES
    src/imgui/imgui.cpp
    src/imgui/imgui_demo.cpp
    src/imgui/imgui_draw.cpp
    src/imgui/imgui_tables.cpp
    src/imgui/imgui_widgets.cpp
    src/imgui/imgui_impl_glfw.cpp
    src/imgui/imgui_impl_opengl3.cpp
)

file(GLOB_RECURSE GUI_SOURCES "src/gui/*.cpp")
file(GLOB_RECURSE RENDER_SOURCES "src/gui/renderer/*.cpp")

add_executable(${PROJECT_NAME} 
    ${CORE_SOURCES} 
    ${GUI_SOURCES} 
    ${IMGUI_SOURCES}
    ${RENDER_SOURCES}
)
target_include_directories(${PROJECT_NAME} PRIVATE 
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src/imgui
)

target_link_libraries(${PROJECT_NAME} PRIVATE 
    OpenGL::GL
    glfw
)