#pragma once
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

class ImGuiHalloWelt
{
private:
    /* data */
public:
    ImGuiHalloWelt(/* args */);
    ~ImGuiHalloWelt();

    void imGuiInit(GLFWwindow *window);
    void drawImGui();
    void killImGui();

};

ImGuiHalloWelt::ImGuiHalloWelt(/* args */)
{
}

ImGuiHalloWelt::~ImGuiHalloWelt()
{
}


void ImGuiHalloWelt::imGuiInit(GLFWwindow *window){
        IMGUI_CHECKVERSION();
	ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
	// Enable Docking
    io.ConfigFlags |= 0;
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");
	// Load Fonts
    io.Fonts->AddFontDefault();
    ImGui::GetStyle().ScaleAllSizes(1.2);
}

void ImGuiHalloWelt::drawImGui(){
    // Starte ImGui-Frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();        
    ImGui::NewFrame();
    
    // Hier kannst du ImGui-Benutzeroberfläche erstellen
    ImGui::Begin("Hello, world!");
    ImGui::Text("This is some useful text.");
    ImGui::End();
    
    // Rendere ImGui
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ImGuiHalloWelt::killImGui(){
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}