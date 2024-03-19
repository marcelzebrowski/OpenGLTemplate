#include <iostream>
#include <fstream>
#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <sstream>

#define ASSERT(x) if (!(x)) __debugbreak();
#define GlCall(x) {\
    GlClearError();\
    x;\
    ASSERT(GlLogCall(#x, __FILE__, __LINE__))\
}

struct ShaderProgramSource{
    std::string VertexSource;
    std::string FragmentSource;
};

static void GlClearError(){
    while (glGetError() != GL_NO_ERROR)
    {
        /* code */
    }
}

static GLboolean GlLogCall(const char* function, const char* file, int line){
    while (GLenum error = glGetError())
    {
        /* code */
        std::cout << "[OpenGL Error] (" << error << ")" 
        << " file: " << file
        << " function " << function << " line " 
        << line << std::endl;
        return GL_FALSE;
    }
    return GL_TRUE;
    
}

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

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    /* we need a valid context! */
    if(glewInit() != GLEW_OK){
        std::cout << "Error! I was not able to initialice glew." << std::endl;
        return -1;
    }else{
        std::cout << glGetString(GL_VERSION) << std::endl;
    }

    GLfloat positions[] = {
        -0.5f, -0.5f, // 0
         0.5f, -0.5f, // 1
         0.5f,  0.5f, // 2
        -0.5f,  0.5f  // 3
    }; 

    GLuint index[] = {
     0,1,2,
     2,3,0
    };

    // create vertex buffer
    GLuint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, 6 * 2* sizeof(GLfloat), positions, GL_STATIC_DRAW);

    glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE,2 * sizeof(GLfloat),0);
    glEnableVertexAttribArray(0);

    // create index buffer
    GLuint ibo;
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(GLuint), index, GL_STATIC_DRAW);


    ShaderProgramSource source = ParseShader("shader/basic_Shader.glsl");

    GLuint shader = CreateShader(source.VertexSource, source.FragmentSource); 
    glUseProgram(shader);


    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {

        GLuint buffer;
        GLsizei n = 3;
        glGenBuffers(n, &buffer);
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);
        

		/*glBegin(GL_TRIANGLES);
        glColor3f(1.0f,1.0f,0.0f);
		glVertex2f(-0.5f, -0.5f);
        glColor3f(1.0f,0.0f,0.0f);
		glVertex2f(0.0f, 0.5f);
        glColor3f(0.0f,1.0f,1.0f);
		glVertex2f(0.5f, -0.5f);
		glEnd();*/

        //glDrawArrays(GL_TRIANGLES,0,6); // affected by glBindBuffers
        GlCall(glDrawElements(GL_TRIANGLES,6, GL_UNSIGNED_INT, nullptr));
        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glDeleteProgram(shader);

    glfwTerminate();
    return 0;
}