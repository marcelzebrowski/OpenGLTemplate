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
#include <memory>

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
#include "Text.hpp"
#include "Picture.hpp"
#include "PictureFadeController.hpp"
#include "AudioManager.hpp"
#include "PictureAnimator.hpp"
#include "PictureAnimatorManager.hpp"
#include "ScrollingText.hpp"
#include "D20Wireframe.hpp"
#include "D20WireframeAnimator.hpp"
#include "Scene.hpp"
#include "FraktalEffect.hpp"
#include "FadeEffect.hpp"

#define M_PI 3.14159265358979323846


float delta;
float elapsed;
int maxWidth = 1920;
int maxHeight = 1080;
float lastX = (float)maxWidth / 2;
float lastY = (float)maxHeight / 2;
const float sensivity = 0.1f;
float yaw = -90.0f;
float pitch = 0.0f;
float fov = 45.0f;
bool firstMouseMove = true;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f),glm::vec3(0.0f, 0.0f, -1.0f),glm::vec3(0.0f, 1.0f, 0.0f));
ViewportHandler viewportHandler;

AudioManager audioManager;



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


std::string flattenText(const char* rawText){
	std::string result;

	while(*rawText){
		if(*rawText != '\n' && *rawText != '\r'){
			result += *rawText;
		}
		rawText++;
	}
	return result;
}




int main(void) {

	// initialize and configure
	glfwInit();

	// fullscreen
	//GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();

	int count;
	GLFWmonitor** monitors = glfwGetMonitors(&count);
	GLFWmonitor* monitor;
	if(count > 0){
		monitor = monitors[0];
	} else {
		monitor = glfwGetPrimaryMonitor();
	}
	 
	
	const GLFWvidmode* mode = glfwGetVideoMode(monitor);
	maxWidth = mode->width;
	maxHeight = mode->height;

	float aspectRatio;

	glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	#ifdef __APPLE__
   		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	#endif


	// create window
	GLFWwindow* window;

	#ifndef NDEBUG
		// Debug-Modus: Windowed
		maxWidth = 1920;
		maxHeight = 1080;
		window = glfwCreateWindow(maxWidth, maxHeight, "Final First 2026", NULL, NULL);
	#else
		// Release-Modus: Fullscreen
		window = glfwCreateWindow(maxWidth, maxHeight, "Final First 2026", monitor, NULL);
	#endif

	aspectRatio = static_cast<float>(maxWidth/maxHeight);
	
	if (window == NULL){
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}else {
		std::cout << "width: " << maxWidth << " height: " << maxHeight << std::endl;
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

	// play sound
	audioManager.PlaySong("sound/m4rc0-they-live.mp3");


	glfwSwapInterval(1); 

	glEnable(GL_DEPTH_TEST);

	{
		MarcelsTimer timer;

		Shader fraktalShader("shader/fraktal/fraktal_vs.glsl","shader/fraktal/fraktal_fs.glsl");
		Shader pictureShader("shader/picture/vertex.glsl","shader/picture/fragment.glsl");
		
		// initial window registration
		viewportHandler.registerWithWindow(window);

		int width, height;
		glfwGetFramebufferSize(window, &width,&height);
		viewportHandler.framebufferSizeCallBack(width,height);

	
		// Fraktal Scene
		Fraktal fraktal(&fraktalShader, maxHeight, maxWidth);

		auto fraktalEffect = std::make_unique<FraktalEffect>(&fraktal,0.0f, 60.0f);
		auto fadeEffect = std::make_unique<FadeEffect>(std::move(fraktalEffect), 0.0f,5.0f, true);
		
		Scene fraktalScene01;
		fraktalScene01.addEffect(std::move(fadeEffect));

		while(!glfwWindowShouldClose(window)){
			delta = (float)timer.delta();

			processInput(window);

			audioManager.Update(delta);

			glClearStencil(0);
			glClear(GL_STENCIL_BUFFER_BIT);
			glClearColor(0.1f,0.1f,0.1f,1.0f);
			glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT| GL_STENCIL_BUFFER_BIT);
			
			glm::mat4 model = glm::mat4(1.0f);
			glm::mat4 view = camera.getViewMatrix();

			fraktalScene01.update(delta);
			fraktalScene01.render();

			glfwSwapBuffers(window);
			glfwPollEvents();
		}

	}
	
	glfwTerminate();

	audioManager.StopSongs();

	return 0;
}