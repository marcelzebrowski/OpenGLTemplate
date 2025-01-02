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
#include "GLErrorCheck.hpp"
#include "CoordinatesSystem.hpp"
#include "Cube.hpp"
#include "Camera.hpp"

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

float randomColor(){
	float timeValue = (float)glfwGetTime();
	float value = (sin(timeValue*doRandom())/2.0f) + 0.5f;
	return value;
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
		Camera camera;
		Shader coordinateSystemShader("shader/axes_vertex.glsl","shader/axes_fragment.glsl");
		Shader cubeShader("shader/vertex.glsl","shader/fragment.glsl");
		Texture cube0Texture("texture/container.png",0);
		Texture cube1Texture("texture/awesomeface.png",1);


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
		Cube cube(&cubeShader,&cube0Texture, &cube1Texture);

		glm::vec3 cubePositions[] = {
			glm::vec3( 0.0f,  0.0f,  0.0f), 
			glm::vec3( 2.0f,  5.0f, -15.0f), 
			glm::vec3(-1.5f, -2.2f, -2.5f),  
			glm::vec3(-3.8f, -2.0f, -12.3f),  
			glm::vec3( 2.4f, -0.4f, -3.5f),  
			glm::vec3(-1.7f,  3.0f, -7.5f),  
			glm::vec3( 1.3f, -2.0f, -2.5f),  
			glm::vec3( 1.5f,  2.0f, -2.5f), 
			glm::vec3( 1.5f,  0.2f, -1.5f), 
			glm::vec3(-1.3f,  1.0f, -1.5f)  
		};


		while(!glfwWindowShouldClose(window)){
			processInput(window);

			glClearColor(0.5f,0.0f,0.0f,1.0f);
			glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
			
			const float radius = 5.0f;
			float camX = sin(glfwGetTime() / radius);
			float camZ = cos(glfwGetTime() / radius);
			//camera.setPosition(glm::vec3(camX,0.0f,camZ));
			//camera.rotate(glfwGetTime(),glfwGetTime());
			
	
			glm::mat4 model = glm::mat4(1.0f);
		
			glm::mat4 view = camera.getViewMatrix();

			coordinateSystem.render(model,view);

			for(unsigned int i=0; i<10;i++){
				glm::mat4 modelCube = glm::translate(model,cubePositions[i]);
				modelCube = glm::rotate(modelCube, (float)glfwGetTime(),cubePositions[i]); 
				cube.render(modelCube,view, randomColor(), randomColor(), randomColor());
			}

			glfwSwapBuffers(window);
			glfwPollEvents();    
		}

	}
	
	glfwTerminate();

	return 0;
}
