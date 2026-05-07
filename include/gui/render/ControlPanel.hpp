#include "../../core/Board.hpp"
#include "../../core/BoardAlg.hpp"
#include "../../../src/imgui/imgui.h"
#include "../../../src/imgui/imgui_internal.h"
#include "../../../src/imgui/imgui_impl_glfw.h"
#include "../../../src/imgui/imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include <vector>
#include <string>

#define DIRBUFSIZE 128
class ControlPanel{
private:
    Board* board;
    public:
    char dirbuf[DIRBUFSIZE];

public:
    ControlPanel();
    int algorithm; //0=UCS, 1=A*, 2=GBFS 
    int heuristic;
    void loadBoard(std::ifstream& config);
    void solveBoard();
};
    


namespace renderer {
    ImGuiID renderPanel(ControlPanel& cp);
}
