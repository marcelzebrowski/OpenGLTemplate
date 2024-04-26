#pragma once
#include <iostream>
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

class ImGuiHalloWelt
{
private:
    /* data */
    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    float m_rotationSpeed1;
    float m_rotationSpeed2;

public:
    ImGuiHalloWelt(/* args */);
    ~ImGuiHalloWelt();

    void imGuiInit(GLFWwindow *window);
    void drawImGui();
    void killImGui();

    const float& getRotationSpeed1() const{
        return m_rotationSpeed1;    
    }

    const float& getRotationSpeed2() const{
        return m_rotationSpeed2;    
    }

};

ImGuiHalloWelt::ImGuiHalloWelt(/* args */):m_rotationSpeed1(0.1f),m_rotationSpeed2(0.1f)
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
    ImGui::GetStyle().ScaleAllSizes(1.2f);
}

void ImGuiHalloWelt::drawImGui(){
    // Starte ImGui-Frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();        
    ImGui::NewFrame();
    
    //------------------------------------
    // Example Code
    //------------------------------------
    ImGui::Begin("Hello, world!");
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    

    ImGui::Text("Rotation Speed1");               // Display some text (you can use a format strings too)
    ImGui::SameLine();
    ImGui::SliderFloat("Rot1", &m_rotationSpeed1, 0.1f, 10.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
    ImGui::Text("Rotation Speed2");
    ImGui::SameLine();
    ImGui::SliderFloat("Rot2", &m_rotationSpeed2, 0.1f, 10.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
  
    
    
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
    
    ImGui::End();
    
    //--------------------------------------

    
    // Rendere ImGui
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ImGuiHalloWelt::killImGui(){
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}