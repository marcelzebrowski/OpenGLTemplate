#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>


float triangle1[] = {
	0.5f,  0.5f, 0.0f,
	0.5f, -0.5f, 0.0f,
	-0.5f,  0.5f, 0.0f,
};

float triangle2[] = {
	-0.5f, -0.5f, 0.0f,
	-0.5f,  0.5f, 0.0f,
	 0.5f, -0.5f, 0.0f,
};

void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    glViewport(0, 0, width, height);
}  

void processInput(GLFWwindow *window){
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}


void render(const unsigned int VAO, const unsigned int iShaderProgramId){
	glBindVertexArray(VAO);
	glUseProgram(iShaderProgramId);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}

unsigned int createShader(const char* program, GLenum shaderType){
	unsigned int shader = glCreateShader(shaderType);
	glShaderSource(shader, 1, &program, NULL);
	glCompileShader(shader);

	int success;
	char infoLog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

	if(!success){
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	return shader;
}


unsigned int createShaderProgram(const char* vertexShaderSource, const char* fragmentShaderSource){
	unsigned int vertexShader = createShader(vertexShaderSource, GL_VERTEX_SHADER);
	unsigned int fragmentShader = createShader(fragmentShaderSource, GL_FRAGMENT_SHADER);
	
	int success;
	char infoLog[512];
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

	if(!success){
		glGetShaderInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::CREATION_FAILED\n" << infoLog << std::endl;
	}

	// delete shader we no longer need them anymore
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return shaderProgram;
}

unsigned int createTriangle(float* vertices, size_t size){
	// create vertex and index buffer
	unsigned int VAO, VBO;
	glGenVertexArrays(1,&VAO);
	glGenBuffers(1,&VBO);
	
	// bind data
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, size * sizeof(float), vertices, GL_STATIC_DRAW);

	// -- linking vertex attributes (interpret the vertex data)
	glVertexAttribPointer(0,3,GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0); //layout(location = 0)
	glEnableVertexAttribArray(0); // enable a generic vertex attribute array 

	// -- unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

	return VAO;
}


int main(void) {
	// initialize and configure
	glfwInit();
	
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	#ifdef __APPLE__
   		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	#endif

	// create window
	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (window == NULL){
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);  

	// load all opengl function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}    

	// -- create vertex shader
	const char* vertexShaderSource = "#version 330 core\n"
					"layout (location = 0) in vec3 aPos;\n"
					"void main(){ \n"
					"    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
					"}\0";

	// -- create fragment shader
	const char* fragmentShaderSource = "#version 330 core\n"
					"out vec4 FragColor;\n"
					"void main(){ \n"
    				"     FragColor = vec4(1.0f,0.5f, 0.2f, 1.0f);\n"
					"}\0";
	
	const char* fragmentShaderSource2 = "#version 330 core\n"
					"out vec4 FragColor;\n"
					"void main(){ \n"
    				"     FragColor = vec4(1.0f,1.0f, 0.2f, 1.0f);\n"
					"}\0";

	unsigned int iShaderProgramId = createShaderProgram(vertexShaderSource, fragmentShaderSource);
	unsigned int iShaderProgramId2 = createShaderProgram(vertexShaderSource, fragmentShaderSource2);

	unsigned int VAO1 = createTriangle(triangle1,sizeof(triangle1)/sizeof(triangle1[0]));
	unsigned int VAO2 = createTriangle(triangle2,sizeof(triangle2)/sizeof(triangle2[0]));

	while(!glfwWindowShouldClose(window)){
		processInput(window);

		glClearColor(0.2f,0.3f,0.3f,1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		render(VAO1, iShaderProgramId);
		render(VAO2, iShaderProgramId2);

		glfwSwapBuffers(window);
		glfwPollEvents();    
	}

	// clean
	glDeleteVertexArrays(1,&VAO1);
	glDeleteVertexArrays(1,&VAO2);
	glDeleteProgram(iShaderProgramId);

	glfwTerminate();

	return 0;
}