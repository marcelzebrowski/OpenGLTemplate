#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <random>
#include <vector>
#include <cmath>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.hpp"
#include "Texture.hpp"
#include "ViewportHandler.hpp"

#include "CoordinatesSystem.hpp"
#include "Cube.hpp"


#define M_PI 3.14159265358979323846

int doRandom(){
	std::random_device rd; // Liefert einen zufälligen Seed
    std::mt19937 gen(rd()); // Mersenne Twister PRNG initialisiert mit Seed

    // Verteilung festlegen (z. B. Ganzzahlen zwischen 1 und 100)
    std::uniform_int_distribution<> distr(1, 100);

    // Zufallszahl generieren
    return distr(gen);
}

void processInput(GLFWwindow *window){
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
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

	// load all opengl function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}    

	glEnable(GL_DEPTH_TEST);

	{
		Shader coordinateSystemShader("shader/axes_vertex.glsl","shader/axes_fragment.glsl");
		Shader cubeShader("shader/vertex.glsl","shader/fragment.glsl");
		ViewportHandler viewportHandler;
		viewportHandler.addShader(&coordinateSystemShader);
		viewportHandler.addShader(&cubeShader);

		// initial window registration
		viewportHandler.registerWithWindow(window);

		int width, height;
		glfwGetFramebufferSize(window, &width,&height);
		viewportHandler.framebufferSizeCallBack(width,height);

	
	
		// we have to take care that we instantiate our shader after framebufferSizeCallBack!
		CoordinateSystem coordinateSystem(&coordinateSystemShader);
		Cube cube(&cubeShader);


		while(!glfwWindowShouldClose(window)){
			processInput(window);

			glClearColor(0.0f,0.0f,0.0f,1.0f);
			glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
			
			glm::mat4 view = glm::translate(glm::mat4(1.0f),glm::vec3(0.0f,0.0f,-3.0f));
			glm::mat4 model = glm::mat4(1.0f); 
		

			coordinateSystem.render(model,view);
			cube.render(model,view);

			glfwSwapBuffers(window);
			glfwPollEvents();    
		}

	}
	
	glfwTerminate();

	return 0;
}
