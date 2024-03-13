#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

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

    glAttachShader(program, vs);
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

    GLfloat positions[6] = {
        -0.5f, -0.5f,
        0.0f, 0.5f,
        0.5f, -0.5f
    }; 

    // create buffer
    GLuint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(GLfloat), positions, GL_STATIC_DRAW);

    glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE,2 * sizeof(GLfloat),0);
    glEnableVertexAttribArray(0);


    std::string vertexShader = 
        "#version 330 core\n"
        "\n"
        "layout(location = 0) in vec4 position;\n"
        "void main(){\n"
        " gl_Position = position;\n"
        "}\n";
    std::string fragmentShader = 
        "#version 330 core\n"
        "\n"
        "layout(location = 0) out vec4 color;\n"
        "void main(){\n"
        " color = vec4(1.0,0.0,0.0,1.0);\n"
        "}\n";

    GLuint shader = CreateShader(vertexShader, fragmentShader); 
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

        glDrawArrays(GL_TRIANGLES,0,3); // affected by glBindBuffers

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glDeleteProgram(shader);

    glfwTerminate();
    return 0;
}