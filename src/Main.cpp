#include <iostream>
#include <fstream>
#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <sstream>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"


struct ShaderProgramSource{
    std::string VertexSource;
    std::string FragmentSource;
};

static ShaderProgramSource ParseShader(const std::string file){
    
    enum class ShaderType{
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    std::ifstream stream(file);
    
    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;

    while(getline(stream,line)){
        if(line.find("#shader") != std::string::npos){
            if(line.find("vertex") != std::string::npos){
                type = ShaderType::VERTEX;
            }else if (line.find("fragment") != std::string::npos){
                type = ShaderType::FRAGMENT;
            }
        }else{
            ss[(int) type] << line << '\n';
        }
    }

    return {ss[0].str(), ss[1].str()};
}


void printSize(){
    std::cout << "Size of GLfloat:" << sizeof(GLfloat) << std::endl; 
}

static GLuint CompileShader(const std::string& source, GLuint type){
    GLuint id = glCreateShader(type);
    const char* src = &source[0];
    glShaderSource(id,1,&src, nullptr); // 10:33
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);

    if(!result){
        int lenght;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &lenght);
        char* message = (char*)alloca(lenght * sizeof(char));
        glGetShaderInfoLog(id, lenght, &lenght, message);

        std::cout << "Failed to compile "<< (type == GL_VERTEX_SHADER ? "vertex":"fragment") << " shader!" << std::endl; 
        std::cout << message << std::endl;
        glDeleteShader(id);

        return 0;
    }

    return id;
}

static GLuint CreateShader(const std::string& vertexShader, const std::string& fragmentShader){
    GLuint program = glCreateProgram();
    GLuint vs = CompileShader(vertexShader, GL_VERTEX_SHADER);
    GLuint fs = CompileShader(fragmentShader, GL_FRAGMENT_SHADER);

    GlCall( glAttachShader(program, vs)) ;
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);
    return program;
}

int main(void)
{
    printSize();


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

        // create gl vertex array (Hint: glEnableVertexAttribArray/GLFW_OPENGL_CORE_PROFILE)
        GLuint vao;
        GlCall(glGenVertexArrays(1,&vao));
        GlCall(glBindVertexArray(vao));

        // create vertex buffer
        VertexBuffer vb(positions, 4 * 2 * sizeof(GLfloat));


        glEnableVertexAttribArray(0); 
        glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE,2 * sizeof(GLfloat),0); // links the buffer with vao

        // create index buffer
        IndexBuffer ib(indeces,6);

        ShaderProgramSource source = ParseShader("shader/basic_Shader.glsl");

        GLuint shader = CreateShader(source.VertexSource, source.FragmentSource); 
        GlCall(glUseProgram(shader));
        

        GLint location = glGetUniformLocation(shader,"u_Color");
        ASSERT(location != -1);
        glUniform4f(location,0.0f,1.0f,0.0f,1.0f);  
        

        GlCall(glBindVertexArray(0));
        GlCall(glUseProgram(0));
        GlCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
        GlCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

        float r = 0.0f;
        float inc = 0.05f;
        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {

            if(r >= 1.0f){
                inc = -0.05f;
            }else if(r <0.0f){
                inc = 0.05f;
            }

            r +=inc;

            /* Render here */
            glClear(GL_COLOR_BUFFER_BIT);
            
            GlCall(glUseProgram(shader));
            GlCall(glUniform4f(location,r,1.0f,0.0f,1.0f));  
            
            //GlCall(glBindBuffer(GL_ARRAY_BUFFER, buffer));
            //GlCall(glEnableVertexAttribArray(0));
            //GlCall(glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE,2 * sizeof(GLfloat),0));

            // vao and ibo
            GlCall(glBindVertexArray(vao));
            ib.Bind();

            GlCall(glDrawElements(GL_TRIANGLES,6, GL_UNSIGNED_INT, nullptr));

            /*glBegin(GL_TRIANGLES);
            glColor3f(1.0f,1.0f,0.0f);
            glVertex2f(-0.5f, -0.5f);
            glColor3f(1.0f,0.0f,0.0f);
            glVertex2f(0.0f, 0.5f);
            glColor3f(0.0f,1.0f,1.0f);
            glVertex2f(0.5f, -0.5f);
            glEnd();*/

            //glDrawArrays(GL_TRIANGLES,0,6); // affected by glBindBuffers
            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }

        glDeleteProgram(shader);
    }

    glfwTerminate();
    return 0;
}