#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "AudioManager.h"

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
            -0.5f, -0.5f, // 0
            0.5f, -0.5f, // 1
            0.5f,  0.5f, // 2
            -0.5f,  0.5f  // 3
        }; 

        GLuint indeces[] = {
        0,1,2,
        2,3,0
        };

        
  
        
        VertexBuffer vb(positions, 4 * 2 * sizeof(GLfloat));
        IndexBuffer ib(indeces,6);

        VertexBufferLayout layout;
        layout.Push<float>(2);
        
        VertexArray va;
        va.AddBuffer(vb, layout);

        Shader shader("shader/basic_Shader.glsl");
        shader.Bind();
        shader.SetUniform4f("u_Color",0.8f, 0.3f, 0.8f, 1.0f);

        va.Unbind();
        vb.Unbind();
        ib.Unbind();
        shader.Unbind();

        Renderer renderer;

        float r = 0.0f;
        float inc = 0.05f;
        
        float fade = 0.0f;
        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            if(fade < 1.0f){
                fade +=0.0001f;
            }else{
                fade = 1.0f;
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
            
            renderer.Draw(va,ib,shader);
            
            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }

    }

    audioManager.StopSongs();

    glfwTerminate();
    return 0;
}