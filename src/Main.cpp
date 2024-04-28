#include <iostream>
//#include <GL/glew.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>




#include "Renderer.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "AudioManager.h"
#include "Texture.h"

#include "ImGuiHalloWelt.h"


int gladInit(){
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Fehler beim Initialisieren von Glad" << std::endl;
        glfwTerminate();
        return -1;
    }
    return 1;
}

int main(void)
{
    ImGuiHalloWelt imGuiHalloWelt;
    AudioManager audioManager;
    audioManager.LoadSong("sound/13_Mindriot.mp3");
    
    GLFWwindow* window;



    /* Initialize the library */
    if (!glfwInit())
        return -1;

    int width = 1920;
    int height = 1080;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // you have to create a vertex array object!
    window = glfwCreateWindow(width,height, "Hello World", NULL, NULL);
    glfwSetWindowSizeLimits(window, width, height, width, height);
    glfwSetWindowAspectRatio(window, width, height);
    glfwSetWindowAttrib(window, GLFW_RESIZABLE, GLFW_FALSE);
    glfwSetWindowAttrib(window, GLFW_MAXIMIZED, GLFW_FALSE);


    /* Create a windowed mode window and its OpenGL context */
    
    
    
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if(gladInit() == -1){
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }


    imGuiHalloWelt.imGuiInit(window);

    glfwSwapInterval(1);

    /* we need a valid context! */
    if(!glfwInit()){
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


        
        glm::mat4 proj = glm::ortho(-1.0f,1.0f, -0.5f, 0.5f, -1.0f, 1.0f);
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 model = glm::rotate(view,glm::radians(0.0f),glm::vec3(0.0f,0.0f,1.0f));
        glm::mat4 mvp = proj * view * model;

        Shader shader("shader/basic_Shader.vert","shader/basic_Shader.frag");
        shader.Bind();
        shader.SetUniform4f("u_Color",0.8f, 0.3f, 0.8f, 1.0f);
        shader.SetUniformMat4f("u_MVP", mvp);
    
        Texture texture("textur/Ina.jpg");
        texture.Bind();
        shader.SetUniform1i("u_Texture",0);

        va.Unbind();
        vb.Unbind();
        ib.Unbind();
        shader.Unbind();

        Renderer renderer;
 
        float r = 0.0f;
        float inc = 0.05f;
        
        float fade = 0.0f;

        float rotation1 = 0.0f;
        float rotation2 = 0.0f;
        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {

            rotation1 += imGuiHalloWelt.getRotationSpeed1();               
            rotation2 += imGuiHalloWelt.getRotationSpeed2();

            if(fade < 100.0f){
                fade +=0.05f;
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

        
            shader.Bind();
            shader.SetUniform4f("u_Color",r, 0.3f, 0.8f, 1.0f);
            glm::mat4 model1 = glm::rotate(view,glm::radians(rotation1),glm::vec3(0.0f,0.0f,1.0f));
            glm::mat4 mvp1 = proj * view * model1;
            shader.SetUniformMat4f("u_MVP", mvp1);
            renderer.Draw(va,ib,shader);

            glm::mat4 view2 = glm::translate(glm::mat4(1.0f),glm::vec3(0.5f, -0.5f, 0.0f));
            glm::mat4 model2 = glm::rotate(view2,glm::radians(-rotation2),glm::vec3(0.0f,0.0f,1.0f));
            glm::mat4 mvp2 = proj * view * model2;
            shader.SetUniformMat4f("u_MVP", mvp2);
            renderer.Draw(va,ib,shader);

            imGuiHalloWelt.drawImGui();        
      

       
   
            
            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }



        
    }


    imGuiHalloWelt.killImGui();

    audioManager.StopSongs();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}