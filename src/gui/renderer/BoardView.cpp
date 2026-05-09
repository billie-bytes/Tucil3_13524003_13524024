#include "../../../include/gui/render/BoardView.hpp"
#include "../../../include/core/Board.hpp"
#include "../../imgui/imgui.h"
#include "../../imgui/imgui_internal.h"
#include "../../imgui/imgui_impl_glfw.h"
#include "../../imgui/imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <vector>
#include <array>
#include <string>

namespace renderer {

    ImGuiID renderBoard(Board& b){

        int** matrix = b.getMatrix();
        ImGui::Begin("Board");
        ImDrawList* draw_list = ImGui::GetWindowDrawList();
        ImVec2 top_left = ImGui::GetCursorScreenPos();
        ImVec2 canvas_size =  ImGui::GetContentRegionAvail();
        ImVec2 grid_size; 
        float padding_size; //Size of the black-bar/padding to make the canvas stay centered

        // Make sure its square by picking the smaller side
        // Also updates the top_left of the canvas tergantung dengan dimensi windownya
        if(canvas_size.x>canvas_size.y){
            grid_size = {canvas_size.y/b.panjang,canvas_size.y/b.lebar};
            padding_size = (canvas_size.x - grid_size.x*b.lebar)/2;
            top_left.x+=padding_size;
        }
        else{
            grid_size = {canvas_size.x/b.panjang,canvas_size.x/b.lebar};
            padding_size = (canvas_size.y - grid_size.y*b.panjang)/2;
            top_left.y+=padding_size;
        }

        ImVec2 rectTopLeft = top_left;
        ImVec2 rectBotRight = {top_left.x+grid_size.x,top_left.y+grid_size.y};

        for(int i = 0; i<b.panjang; i++){

            for(int j = 0; j<b.lebar; j++){
                if(matrix[i][j] < WALL){
                    draw_list->AddRectFilled(rectTopLeft,rectBotRight,IM_COL32(67,67,67,200));
                    draw_list->AddRect(rectTopLeft,rectBotRight,IM_COL32(40,40,40,200));
                }

                if(matrix[i][j] == LAVA) draw_list->AddRectFilled(rectTopLeft,rectBotRight,IM_COL32(255,0,0,255));
                if(matrix[i][j] == WALL) draw_list->AddRectFilled(rectTopLeft,rectBotRight,IM_COL32(67,67,67,255));
                if(b.pinX==i&&b.pinY==j) draw_list->AddRectFilled(rectTopLeft,rectBotRight,IM_COL32(0,0,255,255));
                if(b.winX==i&&b.winY==j) draw_list->AddRectFilled(rectTopLeft,rectBotRight,IM_COL32(0,255,0,255));
                auto x_it = b.orderedTiles.find(i);
                if (x_it != b.orderedTiles.end()) {
                    auto y_it = x_it->second.find(j);
                    if (y_it != x_it->second.end()) {
                        if(y_it->second!=-1){
                            std::string label = std::to_string(y_it->second);
                            draw_list->AddText(NULL, 50.0f, rectTopLeft, IM_COL32(255,255,255,255), label.c_str());
                        }
                    }
                }

                rectTopLeft.x+=grid_size.x;
                rectBotRight.x+=grid_size.x;
            }
            rectTopLeft.x-=grid_size.x*(b.lebar);
            rectBotRight.x-=grid_size.x*(b.lebar);

            rectTopLeft.y+=grid_size.y;
            rectBotRight.y+=grid_size.y;
        }


        ImGui::End();
        return ImGui::GetID("Board");
    }

}