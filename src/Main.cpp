#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>


#include "Renderer.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "AudioManager.h"
#include "Texture.h"

int main(void)
{

    AudioManager audioManager;
    audioManager.LoadSong("sound/Switchback.mod");
    
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // you have to create a vertex array object!
    

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

   

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glfwSwapInterval(1);

    /* we need a valid context! */
    if(glewInit() != GLEW_OK){
        std::cout << "Error! I was not able to initialice glew." << std::endl;
        return -1;
    }else{
        std::cout << glGetString(GL_VERSION) << std::endl;
    }


    { // for better terminate
        GLfloat positions[] = {
            -0.5f, -0.5f, 0.0f, 0.0f, // 0
            0.5f, -0.5f, 1.0f, 0.0f, // 1
            0.5f,  0.5f, 1.0f, 1.0f,// 2
            -0.5f,  0.5f, 0.0f,1.0f  // 3
        }; 

        GLuint indeces[] = {
        0,1,2,
        2,3,0
        };

        GlCall(glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA));
  
        
        VertexBuffer vb(positions, 4 * 4 * sizeof(GLfloat));
        IndexBuffer ib(indeces,6);

        VertexBufferLayout layout;
        layout.Push<float>(2);
        layout.Push<float>(2);
        
        VertexArray va;
        va.AddBuffer(vb, layout);


        
        glm::mat4 proj = glm::ortho(-2.0f,2.0f, -1.5f, 1.5f, -1.0f, 1.0f);
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 model = glm::rotate(view,glm::radians(0.0f),glm::vec3(0.0f,0.0f,1.0f));
        glm::mat4 mvp = proj * view * model;

        Shader shader("shader/basic_Shader.vert","shader/basic_Shader.frag");
        shader.Bind();
        shader.SetUniform4f("u_Color",0.8f, 0.3f, 0.8f, 1.0f);
        shader.SetUniformMat4f("u_MVP", mvp);
    
        Texture texture("textur/palette.png");
        texture.Bind();
        shader.SetUniform1i("u_Texture",0);

        va.Unbind();
        vb.Unbind();
        ib.Unbind();
        shader.Unbind();

        Renderer renderer;


        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
        ImGui::StyleColorsDark();
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        bool show_demo_window = true;
        bool show_another_window = false;
        ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

        float r = 0.0f;
        float inc = 0.05f;
        
        float fade = 0.0f;
        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            if(fade < 1.0f){
                fade +=0.0001f;
            }else{
                fade = 0.0f;
            }

            audioManager.Update(fade);
            
            if(r >= 1.0f){
                inc = -0.05f;
            }else if(r <0.0f){
                inc = 0.05f;
            }

            r +=inc;

            /* Render here */
            renderer.Clear();

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();
            
            shader.Bind();
            shader.SetUniform4f("u_Color",r, 0.3f, 0.8f, 1.0f);
            model = glm::rotate(view,glm::radians(180.0f * fade * 100),glm::vec3(0.0f,0.0f,1.0f));
            mvp = proj * view * model;
            shader.SetUniformMat4f("u_MVP", mvp);
            
            renderer.Draw(va,ib,shader);

            {
                static float f = 0.0f;
                static int counter = 0;

                ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

                ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
                ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
                ImGui::Checkbox("Another Window", &show_another_window);

                ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
                ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

                if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
                    counter++;
                ImGui::SameLine();
                ImGui::Text("counter = %d", counter);

                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
                ImGui::End();
            }

            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }

    }

    audioManager.StopSongs();

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
    return 0;
}