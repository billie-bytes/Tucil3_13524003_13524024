#include "../../core/Board.hpp"
#include "../../core/BoardAlg.hpp"
#include "../../../src/imgui/imgui.h"
#include "../../../src/imgui/imgui_internal.h"
#include "../../../src/imgui/imgui_impl_glfw.h"
#include "../../../src/imgui/imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include <vector>
#include <string>
#include <utility>

#define DIRBUFSIZE 128
class ControlPanel{
private:
    
    public:
    char dirbuf[DIRBUFSIZE];
    
    std::pair<int, std::vector<Direction>> board_result;
    std::vector<std::pair<int, int>> saved_pin_pos;
    size_t result_idx;


public:
    Board* board;
    ControlPanel();
    int algorithm; //0=UCS, 1=A*, 2=GBFS 
    int heuristic;
    void loadBoard(std::ifstream& config);
    void solveBoard();

    //Solution viewing
    void maju();
    void mundur();
    void reset();

};
    


namespace renderer {
    ImGuiID renderPanel(ControlPanel& cp);
}
