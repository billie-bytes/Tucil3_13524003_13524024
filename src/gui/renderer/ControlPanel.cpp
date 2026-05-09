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
#include <chrono>
#include <fstream>

ControlPanel::ControlPanel()
:board(nullptr),algorithm(0),heuristic(0),board_result(-1,{}),result_idx(0),k(50)
{
    for(int i =0; i<DIRBUFSIZE; ++i){
        dirbuf[i] = '\0';
    }
}

void ControlPanel::loadBoard(std::ifstream& config){
    if(board!=nullptr)delete board;
    board = Board::create(config);
    for(int i =0; i<DIRBUFSIZE; ++i){
        dirbuf[i] = '\0';
    }
    board_result.first = -1;
    board_result.second.clear();
    result_idx = 0;
    saved_pin_pos.clear();
    solve_time_ms = -1.0;
}


void ControlPanel::solveBoard(){
    if(board==nullptr)return;
    auto start_time = std::chrono::high_resolution_clock::now();
    switch(algorithm){
        case 0:
            board_result = ASTAR(*board,heuristic);
            break;
        case 1:
            board_result = UCS(*board);
            break;
        case 2:
            board_result = GBFS(*board, heuristic);
        case 3:
            board_result = BFS(*board);
        case 4:
            board_result = BeamSearch(*board, heuristic, k);
    }
    auto end_time = std::chrono::high_resolution_clock::now();
    solve_time_ms = std::chrono::duration<double, std::milli>(end_time - start_time).count();
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
    auto [x,y] = saved_pin_pos[0];
    board->pinX = x; board->pinY = y;
    result_idx = 0;
}

static void printBoard(Board* board, std::ofstream& outFile){
    for(int i = 0; i < board->panjang; ++i){
            for(int j = 0; j < board->lebar; ++j){
                if(i == board->pinX && j == board->pinY){
                    outFile << 'Z';
                } else if(i == board->winX && j == board->winY){
                    outFile << 'O';
                } else if(board->getMatrix()[i][j] == WALL){
                    outFile << 'X';
                } else if(board->getMatrix()[i][j] == LAVA){
                    outFile << 'L';
                } else {
                    bool isNumbered = false;
                    auto x_it = board->orderedTiles.find(i);
                    if(x_it != board->orderedTiles.end()){
                        auto y_it = x_it->second.find(j);
                        if(y_it != x_it->second.end() && y_it->second != -1){
                            outFile << y_it->second;
                            isNumbered = true;
                        }
                    }
                    
                    if(!isNumbered) {
                        outFile << '.'; 
                    }
                }
            }
            outFile << '\n';
        }
}

void ControlPanel::saveSolution(){
    if(board == nullptr || board_result.first == -1) return;

    std::ofstream outFile("solution.txt");
    if(!outFile.is_open()){
        printf("Failed to open solution.txt\n");
        return;
    }

    reset();

    outFile << "initial board\n";
    printBoard(board, outFile);

    int step = 1;
    while(result_idx < board_result.second.size()){
        Direction d = board_result.second[result_idx];
        std::string dirStr;
        switch(d){
            /*
            Okay, due to the logic on the algorithms, UP is y--, which visually is going to the left
            in the board.. I know this is stupid but fixing the actual logic would break a lot of stuff
            so instead I just adjust it here.
            */
            case Direction::UP:dirStr = "LEFT"; break;
            case Direction::DOWN:dirStr = "RIGHT"; break;
            case Direction::LEFT:dirStr = "UP"; break;
            case Direction::RIGHT:dirStr = "DOWN"; break;
        }

        outFile << "STEP " << step++ << ": " << dirStr << "\n";
        maju();
        printBoard(board, outFile);
    }

    outFile << "\nWAKTU EKSEKUSI: " << solve_time_ms << " ms\n";
    outFile << "TOTAL COST: " << board_result.first << "\n";

    outFile.close();
    reset();
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
        ImGui::InputTextWithHint("##path", "Path relative to executable",cp.dirbuf,IM_ARRAYSIZE(cp.dirbuf));
        ImGui::SameLine();
        if (ImGui::Button("Enter Path",ImVec2(-FLT_MIN, 0.0f))) {
            std::ifstream config(cp.dirbuf);
            cp.loadBoard(config);
        }

        ImGui::TableNextRow();
        ImGui::TableNextColumn();
        ImGui::Text("Algorithm Selection");
        ImGui::RadioButton("A*",&cp.algorithm,0);
        ImGui::RadioButton("UCS",&cp.algorithm,1);
        ImGui::RadioButton("GBFS",&cp.algorithm,2);
        

        ImGui::RadioButton("BFS",&cp.algorithm,3);
        ImGui::RadioButton("BeamSearch",&cp.algorithm,4);

        ImGui::SameLine();
        ImGui::BeginDisabled(cp.algorithm != 4);
        ImGui::InputInt("##k", &cp.k); if (cp.k < 1) cp.k = 1;
        ImGui::EndDisabled();

        ImGui::TableNextRow();
        ImGui::TableNextColumn();
        
        ImGui::Text("Heuristic Selection");

        if(cp.algorithm==2&&cp.heuristic==0){ //GBFS harus milih heuristic
            cp.heuristic = 1;
        }   
        if((cp.algorithm==1||cp.algorithm==3)&&cp.heuristic!=0){ //UCS and BFS gblh milih heuristic
            cp.heuristic = 0;
        }  
        
        ImGui::BeginDisabled(cp.algorithm == 2);
        ImGui::RadioButton("None",&cp.heuristic,0); ImGui::SameLine();
        ImGui::EndDisabled();

        ImGui::BeginDisabled(cp.algorithm == 1||cp.algorithm==3);
        ImGui::RadioButton("Manhattan Distance",&cp.heuristic,1); ImGui::SameLine();
        ImGui::RadioButton("Euclidean Distance",&cp.heuristic,2);
        ImGui::EndDisabled();

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
        

        if (cp.board_result.first != -1 && cp.solve_time_ms >= 0.0) {
            float text_height = ImGui::GetTextLineHeightWithSpacing();
            float window_height = ImGui::GetWindowHeight();
            float button_height = ImGui::GetFrameHeight();
            float current_y = ImGui::GetCursorPosY();
            float padding_y = ImGui::GetStyle().WindowPadding.y;
            float target_y = window_height - 3*text_height - button_height - padding_y;
            
            if (target_y > current_y) {
                ImGui::SetCursorPosY(target_y);
            }
            if(ImGui::Button("Save Solution")){
                cp.saveSolution();
            }
            ImGui::Text("Saved solution will go to ./solution.txt");
            ImGui::Text("Solved in %.3f ms", cp.solve_time_ms);
            ImGui::Text("Cost: %d", cp.board_result.first);
        }
        ImGui::End();

        return ImGui::GetID("Control Panel");
    }
}
