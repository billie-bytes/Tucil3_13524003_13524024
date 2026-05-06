#include "../../core/Board.hpp"
#include "../../../src/imgui/imgui.h"
#include "../../../src/imgui/imgui_internal.h"
#include "../../../src/imgui/imgui_impl_glfw.h"
#include "../../../src/imgui/imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>



namespace renderer {
    ImGuiID renderBoard(Board& b);
}