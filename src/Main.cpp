#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <random>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Shader.hpp"


int doRandom(){
	std::random_device rd; // Liefert einen zufälligen Seed
    std::mt19937 gen(rd()); // Mersenne Twister PRNG initialisiert mit Seed

    // Verteilung festlegen (z. B. Ganzzahlen zwischen 1 und 100)
    std::uniform_int_distribution<> distr(1, 100);

    // Zufallszahl generieren
    return distr(gen);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    glViewport(0, 0, width, height);
}  

void processInput(GLFWwindow *window){
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

unsigned int createTriangle(){
	float vertices[] = {
		// positions + color
		 0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
		 0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
		-0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f  
	};

	unsigned int indeces[] = {
	//	0, 1, 3, // first triangle
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

	//                   index, size,  type,   normalized,  stride,          offset
	glVertexAttribPointer(  0,  3    ,GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)0);// layout(location = 0)
	glVertexAttribPointer(  1,  3    ,GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(3*sizeof(float)));// layout(location = 1)
	glEnableVertexAttribArray(0); 
	glEnableVertexAttribArray(1); 

	// -- unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

	return VAO;
}


int getMaxVertexAttributes(){
	int nrAttributes;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;
	return nrAttributes;
}

float randomColor(){
	float timeValue = (float)glfwGetTime();
	float value = (sin(timeValue*doRandom())/2.0f) + 0.5f;
	return value;
}

void render(const unsigned int VAO){

	glBindVertexArray(VAO);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
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

	getMaxVertexAttributes();

	
	{
		Shader shader("shader/vertex.glsl","shader/fragment.glsl");
		unsigned int VAO = createTriangle();
		

		while(!glfwWindowShouldClose(window)){
			processInput(window);

			glClearColor(0.2f,0.3f,0.3f,1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			float c1 = randomColor();
			float c2 = 0.5f;
			float c3 = 0.3f;

			shader.attach();

			shader.setFloat3("color",c1,c2,c3);
			shader.setFloat("time",(float)glfwGetTime());

		
			render(VAO);
			shader.detach();
			

			glfwSwapBuffers(window);
			glfwPollEvents();    
		}

		// clean
		glDeleteVertexArrays(1,&VAO);
	}
	
	glfwTerminate();

	return 0;
}