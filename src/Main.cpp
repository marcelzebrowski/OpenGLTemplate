#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>


void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    glViewport(0, 0, width, height);
}  

void processInput(GLFWwindow *window){
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}


void render(const unsigned int VAO, const unsigned int iShaderProgramId){
	glClearColor(0.2f,0.3f,0.3f,1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glBindVertexArray(VAO);
	glUseProgram(iShaderProgramId);
	//glDrawArrays(GL_TRIANGLES, 0, 6);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
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

unsigned int createTriangle(){

	/*float vertices[] = {
		// first triangle
		 0.5f,  0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		-0.5f,  0.5f, 0.0f,

		// second triangle
		 0.5f, -0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f,
		-0.5f,  0.5f, 0.0f
	};*/

	float vertices[] = {
		 0.5f,  0.5f, 0.0f, //top right
		 0.5f, -0.5f, 0.0f, //buttom right
		-0.5f, -0.5f, 0.0f, //buttom left
		-0.5f,  0.5f, 0.0f  // top left
	};

	unsigned int indeces[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};


	// create vertex and index buffer
	unsigned int VAO, VBO, EBO;
	glGenVertexArrays(1,&VAO);
	glGenBuffers(1,&VBO);
	glGenBuffers(1, &EBO);
	
	// bind data
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// bind indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indeces), indeces, GL_STATIC_DRAW);
	
	// -- linking vertex attributes (interpret the vertex data)
	glVertexAttribPointer(0,3,GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
	glEnableVertexAttribArray(0); // layout(location = 0)

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
	
	unsigned int iShaderProgramId = createShaderProgram(vertexShaderSource, fragmentShaderSource);
	unsigned int VAO = createTriangle();
	

	while(!glfwWindowShouldClose(window)){
		processInput(window);

		render(VAO, iShaderProgramId);

		glfwSwapBuffers(window);
		glfwPollEvents();    
	}

	// clean
	glDeleteVertexArrays(1,&VAO);
	glDeleteProgram(iShaderProgramId);

	glfwTerminate();

	return 0;
}