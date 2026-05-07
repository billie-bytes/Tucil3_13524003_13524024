#include "../../../include/gui/render/ControlPanel.hpp"
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


ControlPanel::ControlPanel(){
    board = nullptr;
    algorithm = 0;
    heuristic = 0;
    for(int i =0; i<DIRBUFSIZE; ++i){
        dirbuf[i] = '\0';
    }
}

void ControlPanel::loadBoard(std::ifstream& config){



}
void ControlPanel::solveBoard(){

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
        if (ImGui::Button("Enter Path")) {

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
        
        ImGui::RadioButton("H1",&cp.heuristic,0); ImGui::SameLine();
        ImGui::RadioButton("H2",&cp.heuristic,1); ImGui::SameLine();
        ImGui::RadioButton("H3",&cp.heuristic,2);

        ImGui::EndTable();


        ImGui::End();

        return ImGui::GetID("Control Panel");
    }
}
