#include "../../../include/gui/render/ControlPanel.hpp"
#include "../../../include/core/Board.hpp"
#include "../../../include/core/BoardAlg.hpp"
#include "../../imgui/imgui.h"
#include "../../imgui/imgui_internal.h"
#include "../../imgui/imgui_impl_glfw.h"
#include "../../imgui/imgui_impl_opengl3.h"
#include "../../imgui/ImGuiFileDialog.h"
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <vector>
#include <array>
#include <string>


ControlPanel::ControlPanel()
:board(nullptr),algorithm(0),heuristic(0),board_result(-1,{}),result_idx(0)
{
    for(int i =0; i<DIRBUFSIZE; ++i){
        dirbuf[i] = '\0';
    }
}

void ControlPanel::loadBoard(std::ifstream& config){
    if(board!=nullptr)delete board;
    board = Board::create(config);
    board_result.first = -1;
    board_result.second.clear();
    result_idx = 0;
    saved_pin_pos.clear();
}


void ControlPanel::solveBoard(){
    if(board==nullptr)return;
    switch(algorithm){
        case 0:
            board_result = ASTAR(*board,heuristic);
            break;
        case 1:
            board_result = UCS(*board,heuristic);
            break;
        case 2:
            board_result = GBFS(*board, heuristic);
    }
    saved_pin_pos.push_back({board->pinX,board->pinY});
}

void ControlPanel::maju(){
    if(board_result.first==-1)return;
    if(result_idx==board_result.second.size()) return;
    board->move(board_result.second[result_idx++]);
    saved_pin_pos.push_back({board->pinX,board->pinY});
}

void ControlPanel::mundur(){
    if(board_result.first==-1)return;
    if(result_idx==0) return;
    auto [x,y] = saved_pin_pos[--result_idx];
    board->pinX = x; board->pinY = y;
}

void ControlPanel::reset(){
    while(result_idx>0){
        mundur();
    }
}



namespace renderer {
    ImGuiID renderPanel(ControlPanel& cp){
        ImGui::Begin("Control Panel");
        ImGui::BeginTable("Input",1);
        ImGui::TableNextRow();
        ImGui::TableNextColumn();
        ImGui::Text("Config File Path");

        ImGui::TableNextRow();
        ImGui::TableNextColumn();
        ImGui::InputText("##path",cp.dirbuf,IM_ARRAYSIZE(cp.dirbuf));
        ImGui::SameLine();
        if (ImGui::Button("Enter Path",ImVec2(-FLT_MIN, 0.0f))) {
            std::ifstream config(cp.dirbuf);
            cp.loadBoard(config);
            for(int i =0; i<DIRBUFSIZE; ++i){
                cp.dirbuf[i] = '\0';
            }
        }

        ImGui::TableNextRow();
        ImGui::TableNextColumn();
        ImGui::Text("Algorithm Selection");
        ImGui::RadioButton("A*",&cp.algorithm,0); ImGui::SameLine();
        ImGui::RadioButton("UCS",&cp.algorithm,1); ImGui::SameLine();
        ImGui::RadioButton("GBFS",&cp.algorithm,2);

        ImGui::TableNextRow();
        ImGui::TableNextColumn();
        
        ImGui::Text("Heuristic Selection");
        ImGui::RadioButton("None",&cp.heuristic,0); ImGui::SameLine();
        ImGui::RadioButton("Manhattan Distance",&cp.heuristic,1); ImGui::SameLine();
        ImGui::RadioButton("Euclidean Distance",&cp.heuristic,2);

        ImGui::BeginDisabled(cp.board==nullptr);
        if (ImGui::Button("Solve Board",ImVec2(-FLT_MIN, 0.0f))) {
            cp.solveBoard();
        }
        ImGui::EndDisabled();

        ImGui::BeginDisabled(cp.board_result.first==-1);

        ImGui::BeginDisabled(cp.result_idx==0);
        if (ImGui::Button("<-")){
            cp.mundur();
        } ImGui::SameLine();
        ImGui::EndDisabled();

        float next_button_width = ImGui::CalcTextSize("->").x + ImGui::GetStyle().FramePadding.x * 2.0f;
        float reset_width = ImGui::GetContentRegionAvail().x - next_button_width - ImGui::GetStyle().ItemSpacing.x;
        if (ImGui::Button("Reset",ImVec2(reset_width, 0.0f))){
            cp.reset();
        } ImGui::SameLine();

        ImGui::BeginDisabled(cp.result_idx==cp.board_result.second.size());
        if (ImGui::Button("->")){
            cp.maju();
        }
        ImGui::EndDisabled();
        ImGui::EndDisabled();

        ImGui::TableNextRow();
        ImGui::TableNextColumn();

        ImGui::EndTable();
        ImGui::End();

        return ImGui::GetID("Control Panel");
    }
}
