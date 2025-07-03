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
#include "Text.hpp"
#include "Picture.hpp"
#include "PictureFadeController.hpp"
#include "AudioManager.hpp"
#include "PictureAnimator.hpp"

#define M_PI 3.14159265358979323846


float delta;
float elapsed;
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
	GLFWwindow* window;

	#ifndef NDEBUG
		// Debug-Modus: Windowed
		maxWidth = 1024;
		maxHeight = 768;
		window = glfwCreateWindow(maxWidth, maxHeight, "LearnOpenGL", NULL, NULL);
	#else
		// Release-Modus: Fullscreen
		window = glfwCreateWindow(maxWidth, maxHeight, "LearnOpenGL", primaryMonitor, NULL);
	#endif

	
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
	audioManager.PlaySong("sound/y2k-1_experience.mod");


	glfwSwapInterval(1); 

	glEnable(GL_DEPTH_TEST);

	{
		MarcelsTimer timer;

		Shader fraktalShader("shader/fraktal/fraktal_vs.glsl","shader/fraktal/fraktal_fs.glsl");
		viewportHandler.addShader(&fraktalShader);
		Shader coordinateSystemShader("shader/axes/axes_vertex.glsl","shader/axes/axes_fragment.glsl");
		viewportHandler.addShader(&coordinateSystemShader);
		Shader pictureShader("shader/picture/vertex.glsl","shader/picture/fragment.glsl",true);
		viewportHandler.addShader(&pictureShader);
		Shader pictureWobbleShader("shader/picture/vertex.glsl","shader/picture/fragment_wobbel.glsl",true);
		viewportHandler.addShader(&pictureWobbleShader);
		Shader textShader("shader/text/vertex.glsl","shader/text/fragment.glsl",true);
		viewportHandler.addShader(&textShader);

		// initial window registration
		viewportHandler.registerWithWindow(window);

		int width, height;
		glfwGetFramebufferSize(window, &width,&height);
		viewportHandler.framebufferSizeCallBack(width,height);

	
		CoordinateSystem coordinateSystem(&coordinateSystemShader);
	

		Texture pictureTexture("texture/umbreon.png",0);
		Texture pictureNerdvana("texture/Nerdvana2.png",0);
		Texture textTexture("texture/ASCII.png",0);

		Picture picture(&pictureShader, &pictureTexture);
		Picture logo (&pictureWobbleShader, &pictureNerdvana);

		PictureFadeController pictureFadeController(&picture);
		PictureFadeController logoFadeController(&logo);

		PictureAnimator pictureAnimator(1024.0f,1536.0f,2.0f, &pictureFadeController);
		pictureAnimator.setTargetSize((float)maxWidth, (float)maxHeight);
		pictureAnimator.start();

		PictureAnimator logoAnimator(1536.0f,1024.0f,2.0f, &logoFadeController,AnimationType::Swing);
		logoAnimator.setTargetSize(maxWidth/2.0f, maxHeight/2.0f);
		logoAnimator.start();


		Fraktal fraktal(&fraktalShader, maxHeight, maxWidth);


		Text text(&textShader,&textTexture);

		float frak = 0.0f;
		while(!glfwWindowShouldClose(window)){
			delta = (float)timer.delta();
			timer.printStats();
			
			processInput(window);

			audioManager.Update(delta);

			glClearStencil(0);
			glClear(GL_STENCIL_BUFFER_BIT);
			glClearColor(0.1f,0.1f,0.1f,1.0f);
			glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT| GL_STENCIL_BUFFER_BIT);
			
			glm::mat4 model = glm::mat4(1.0f);
			glm::mat4 view = camera.getViewMatrix();

			frak += delta/2;
			fraktal.render(frak);

			// Picture
			pictureAnimator.render(delta);

			logoAnimator.render(delta);


			// coordinate
			model = glm::mat4(1.0f);
			view = camera.getViewMatrix();
			coordinateSystem.render(model,view);

			// todo Text rendering
			text.render("Nerdvana!",glm::vec2(0.0f,0.0f),50.0f);





			glfwSwapBuffers(window);
			glfwPollEvents();
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}

	}
	
	glfwTerminate();

	audioManager.StopSongs();

	return 0;
}