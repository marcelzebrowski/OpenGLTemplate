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
#include <thread>

#include "Shader.hpp"
#include "Texture.hpp"
#include "ViewportHandler.hpp"
#include "GLErrorCheck.hpp"
#include "CoordinatesSystem.hpp"
#include "Cube.hpp"
#include "Camera.hpp"
#include "MarcelsTimer.hpp"
#include "LightSource.hpp"

#define M_PI 3.14159265358979323846

float delta;
int maxWidth = 1024;
int maxHeight = 768;
float lastX = (float)maxWidth / 2;
float lastY = (float)maxHeight / 2;
const float sensivity = 0.1f;
float yaw = -90.0f;
float pitch = 0.0f;
float fov = 45.0f;
bool firstMouseMove = true;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f),glm::vec3(0.0f, 0.0f, -1.0f),glm::vec3(0.0f, 1.0f, 0.0f));
ViewportHandler viewportHandler;

int doRandom(){
	std::random_device rd; // Liefert einen zufälligen Seed
    std::mt19937 gen(rd()); // Mersenne Twister PRNG initialisiert mit Seed

    // Verteilung festlegen (z. B. Ganzzahlen zwischen 1 und 100)
    std::uniform_int_distribution<> distr(1, 100);

    // Zufallszahl generieren
    return distr(gen);
}

void mouse_call_back(GLFWwindow* window, double xpos, double ypos){

	if(firstMouseMove){
		lastX = (float)xpos;
		lastY = (float)ypos;
		firstMouseMove = false;
	}

	float xOffset = (float)xpos - lastX;
	float yOffset = (float)ypos - lastY;

	lastX = (float)xpos;
	lastY = (float)ypos;

	xOffset *= sensivity;
	yOffset *= sensivity;

	yaw += xOffset;
	pitch += yOffset;

	if(pitch > 89.0f){
		pitch = 89.0f;
	}
	if(pitch < -89.0f){
		pitch = -89.0f;
	}

	camera.rotate(yaw,pitch);
}

void scroll_back(GLFWwindow *window, double xoffset, double yoffset){

	fov -= (float) yoffset;

	if(fov < 1.0f){
		fov = 1.0f;
	}
	if(fov > 45.0f){
		fov = 45.0f;
	}

	viewportHandler.setFov(fov);
	viewportHandler.framebufferSizeCallBack(maxWidth,maxHeight); // update perspective and shaders
}

void processInput(GLFWwindow *window){
	const float camSpeed = 2.5f * delta;

    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window, true);
	}

	if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
		camera.setPosition(camera.getPosition() + (camSpeed * camera.getTarget()));
	}

	if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
     	camera.setPosition(camera.getPosition() - (camSpeed * camera.getTarget()));
	}

	if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
		camera.setPosition(camera.getPosition() - glm::normalize(glm::cross(camera.getTarget(),camera.getUp())) * camSpeed);
	}

	if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
     	camera.setPosition(camera.getPosition() + glm::normalize(glm::cross(camera.getTarget(),camera.getUp())) * camSpeed);
	}

	
}

float randomColor(){
	float timeValue = (float)glfwGetTime();
	float value = (sin(timeValue*doRandom())/2.0f) + 0.5f;
	return value;
}

int main(void) {

	// initialize and configure
	glfwInit();

	// fullscreen
	GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
	
	const GLFWvidmode* mode = glfwGetVideoMode(primaryMonitor);
	maxWidth = mode->width;
	maxHeight = mode->height;
	
	glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	#ifdef __APPLE__
   		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	#endif

	// create window
	GLFWwindow* window = glfwCreateWindow(maxWidth, maxHeight, "LearnOpenGL", NULL, NULL);
	
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

	// configure mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window,mouse_call_back);
	glfwSetScrollCallback(window, scroll_back);    

	glfwSwapInterval(1); 

	glEnable(GL_DEPTH_TEST);

	{
		Shader coordinateSystemShader("shader/axes/axes_vertex.glsl","shader/axes/axes_fragment.glsl");
		Shader cubeShader("shader/cube/vertex.glsl","shader/cube/fragment.glsl");
		Shader lampShader("shader/lamp/vertex.glsl","shader/lamp/fragment.glsl");
		Shader lightShader("shader/light/phong_vs.glsl","shader/light/phong_fs.glsl");

		Texture cube0Texture("texture/container.png",0);
		Texture cube1Texture("texture/awesomeface.png",1);


		
		viewportHandler.addShader(&coordinateSystemShader);
		viewportHandler.addShader(&cubeShader);
		viewportHandler.addShader(&lampShader);
		viewportHandler.addShader(&lightShader);

		// initial window registration
		viewportHandler.registerWithWindow(window);

		int width, height;
		glfwGetFramebufferSize(window, &width,&height);
		viewportHandler.framebufferSizeCallBack(width,height);

	
	
		// we have to take care that we instantiate our shader after framebufferSizeCallBack!
		CoordinateSystem coordinateSystem(&coordinateSystemShader);
		Cube cube(&lightShader,&cube0Texture, &cube1Texture);

		LightSource lightSource(&lampShader);

		glm::vec3 cubePositions[] = {
			glm::vec3( 0.1f, 0.1f, 0.1f), // main cube
			glm::vec3( 1.0f, 1.0f, 1.0f)  // light source (light position vector) 
		};

		float radius = 10.0f;    // Radius der Kreisbahn
		float baseHeight = 5.0f;     // Fixe Höhe
		float heightAmplitude = 2.0f;
		float angle = 0.0f;      // Startwinkel in Grad
		float speed = 35.0f;  
		
		
		MarcelsTimer timer;
		
		while(!glfwWindowShouldClose(window)){
			delta = (float)timer.delta();
			timer.printStats();
			
			processInput(window);

			glClearColor(0.2f,0.2f,0.2f,1.0f);
			glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
			
			glm::mat4 model = glm::mat4(1.0f);
			glm::mat4 view = camera.getViewMatrix();

			coordinateSystem.render(model,view);

			// light
			glm::vec3 lightPosition = cubePositions[1];
			lightPosition.x = 1.0f + float(sin(glfwGetTime()))*2.0f;
			lightPosition.y = float(sin(glfwGetTime()/2.0f));

			// cube
			glm::mat4 modelCube = glm::translate(model,cubePositions[0]);
			modelCube = glm::rotate(modelCube, (float)glfwGetTime(),glm::vec3(1.0f,1.0f,-1.0f)); 
			cube.render(modelCube,view, lightPosition,camera.getPosition());

			
			
			glm::mat4 modelLight = glm::translate(model,lightPosition); 
			modelLight = glm::scale(modelLight,glm::vec3(0.2f,0.2f,0.2f));
			lightSource.render(modelLight,view);

			glfwSwapBuffers(window);
			glfwPollEvents();
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}

	}
	
	glfwTerminate();

	return 0;
}
