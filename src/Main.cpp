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
#include "Fraktal.hpp"
#include "Model.hpp"

#include "Square.hpp"


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

std::vector<LightSource*> lightSources;

float randomLightColor() {
    static std::random_device rd;  // Seed für Zufallsgenerator
    static std::mt19937 gen(rd()); // Mersenne-Twister
    static std::uniform_real_distribution<float> dis(0.0f, 1.0f);
    return dis(gen);
}

void setupLights(Shader* shader){
	std::vector<glm::vec3> pointLightPositions = {
		glm::vec3( 0.7f,  0.2f,  2.0f),
		glm::vec3( 2.3f, -3.3f, -4.0f),
		glm::vec3(-4.0f,  2.0f, -12.0f),
		glm::vec3( 0.0f,  0.0f, -3.0f)
	};

	for(const auto& position : pointLightPositions){
		LightSource* lightSource = new LightSource(shader);
		lightSource->setPosition(position);

		float r = randomLightColor();
		float g = randomLightColor();
		float b = randomLightColor();
		lightSource->setAmbientColor(glm::vec3(r,g,b) * 0.4f);
		lightSource->setSpecularColor(glm::vec3(1.0f,1.0f,1.0f));
		lightSource->setDiffuseColor(glm::vec3(r,g,b));

		std::cout << "LightColor: " << r << " " << g << " " << b << std::endl;
		lightSources.push_back(lightSource);
	}
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
	glfwWindowHint(GLFW_DEPTH_BITS, 24);
	glfwWindowHint(GLFW_STENCIL_BITS, 8);
	#ifdef __APPLE__
   		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	#endif

	// create window
	GLFWwindow* window;

	#ifndef NDEBUG
		// Debug-Modus: Windowed
		maxWidth = 1024;
		maxHeight = 1024;
		window = glfwCreateWindow(maxWidth, maxHeight, "LearnOpenGL", NULL, NULL);
	#else
		// Release-Modus: Fullscreen
		window = glfwCreateWindow(maxWidth, maxHeight, "LearnOpenGL", primaryMonitor, NULL);
	#endif

	
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
	#ifdef NDEBUG
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	#endif
	glfwSetCursorPosCallback(window,mouse_call_back);
	glfwSetScrollCallback(window, scroll_back);    

	glfwSwapInterval(1); 

	glEnable(GL_DEPTH_TEST);

	{

		Shader stencilShader("shader/stencil/stencil_vertex.glsl","shader/stencil/stencil_fragment.glsl");
		Shader coordinateSystemShader("shader/axes/axes_vertex.glsl","shader/axes/axes_fragment.glsl");


		Shader cubeShader("shader/cube/vertex.glsl","shader/cube/fragment.glsl");
		Shader lightShader("shader/light/phong_vs.glsl","shader/light/phong_fs.glsl");
		Shader lampShader("shader/lamp/vertex.glsl","shader/lamp/fragment.glsl");

		Shader lightSourceShader("shader/light/phong_vs.glsl","shader/light/phong_fs.glsl");

		Texture cube0Texture("texture/container2.png",0);
		Texture cube1Texture("texture/container2_specular.png",1);
		Texture emissionTexture("texture/matrix.jpg",2);

		setupLights(&lampShader);

		viewportHandler.addShader(&coordinateSystemShader);
		viewportHandler.addShader(&cubeShader);
		viewportHandler.addShader(&lampShader);
		viewportHandler.addShader(&lightShader);
		viewportHandler.addShader(&stencilShader);

		// initial window registration
		viewportHandler.registerWithWindow(window);

		int width, height;
		glfwGetFramebufferSize(window, &width,&height);
		viewportHandler.framebufferSizeCallBack(width,height);

		Square square(&stencilShader,glm::vec3(0.4f,0.6f,0.4f),0.4f);
		glm::vec3 red = glm::vec3(1.0f,0.0f,0.0f);
		glm::vec3 green = glm::vec3(0.0f,1.0f,0.0f);
		CoordinateSystem coordinateSystem(&coordinateSystemShader);

		Cube cube(&lightShader,&camera, {&cube0Texture, &cube1Texture, &emissionTexture});
		LightSource lightSource(&lampShader);
		lightSource.setAmbientColor(glm::vec3(1.0f,1.0f,1.0f));
		
		MarcelsTimer timer;

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
    	};\
		glClearColor(0.0f,0.0f,0.0f,1.0f);

		while(!glfwWindowShouldClose(window)){
			
			glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT| GL_STENCIL_BUFFER_BIT);
			delta = (float)timer.delta();
			timer.printStats();
			
			processInput(window);

			glm::mat4 model = glm::mat4(1.0f);
			glm::mat4 view = camera.getViewMatrix();

			glDisable(GL_STENCIL_TEST);
			coordinateSystem.render(model,view);

			glEnable(GL_STENCIL_TEST); // 1. step

			// 2. step - write mask to stencil buffer
			glStencilFunc(GL_ALWAYS, 1, 0xFF); // all fragments will pass the stencil test and if it is equal to 1
			glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE); // replace with 1
			glStencilMask(0xFF);

			// no color
			glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
			glDepthMask(GL_FALSE);
			square.setScale(0.51f);
			square.render(model, view);

			// activate color
			glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
			glDepthMask(GL_TRUE);

			glStencilFunc(GL_EQUAL,1,0xFF);
			glStencilMask(0x00); // don't write to stencil buffer
			//square.setColor(red);
			//square.setScale(0.52f);
			//square.render();

			// ---
			// light
			glm::vec3 lightPosition = glm::vec3( 1.0f, 1.0f, 1.0f);
			lightPosition.x = 1.0f + float(sin(glfwGetTime()))*2.0f;
			lightPosition.y = float(sin(glfwGetTime()/2.0f));


			for(int i = 0; i < 10; i++){
				// cube
				glm::mat4 modelCube = glm::translate(model,cubePositions[i]);
				modelCube = glm::rotate(modelCube, (float)glfwGetTime(),glm::vec3(1.0f,1.0f,-1.0f)); 
				cube.render(modelCube,view, lightSources,(float)glfwGetTime());
			}
			
			
			glm::mat4 modelLight = glm::translate(model,lightPosition); 
			modelLight = glm::scale(modelLight,glm::vec3(0.2f,0.2f,0.2f));
			lightSource.render(modelLight,view);


			for(int i = 0; i < lightSources.size(); i++){
				glm::mat4 modelLamp = glm::translate(model,lightSources[i]->getPosition());
				modelLamp = glm::scale(modelLamp,glm::vec3(0.2f,0.2f,0.2f));
				lightSources[i]->render(modelLamp,view);
			}
			// -----
			
			// render normal
			/*glStencilFunc(GL_EQUAL, 1, 0xFF);
       		glStencilMask(0x00);
			square.setColor(green);
			square.setScale(0.4f);
			square.render();*/

			

			
			
			
			glfwSwapBuffers(window);
			glfwPollEvents();
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}

	}
	
	glfwTerminate();

	return 0;
}

