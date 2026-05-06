#include "../../core/Board.hpp"
#include "../../core/BoardAlg.hpp"
#include "../../../src/imgui/imgui.h"
#include "../../../src/imgui/imgui_internal.h"
#include "../../../src/imgui/imgui_impl_glfw.h"
#include "../../../src/imgui/imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include <vector>
#include <string>


class ControlPanel{
    
public:
    void loadBoard(Board& b, std::ifstream config);
    void solveBoard(Board& b, std::string algorithm, std::string heuristic);
};
    


namespace renderer {
    ImGuiID renderPanel();
}
