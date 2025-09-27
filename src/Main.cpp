#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "utils/MarcelsTimer.hpp"
#include "utils/AudioManager.hpp"
#include "utils/RessourceManager.hpp"
#include "core/SceneFactory.hpp"
#include "core/ViewportHandler.hpp"
#include "core/Camera.hpp"
#include "core/SceneManager.hpp"

float delta;
float elapsed;
int maxWidth = 1920;
int maxHeight = 1080;
float lastX = (float)maxWidth / 2;
float lastY = (float)maxHeight / 2;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f),glm::vec3(0.0f, 0.0f, -1.0f),glm::vec3(0.0f, 1.0f, 0.0f));
ViewportHandler viewportHandler;
AudioManager audioManager;

void processInput(GLFWwindow *window){
	const float camSpeed = 2.5f * delta;
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window, true);
	}
}


void initRessource(){
	RessourceManager::loadShader(ShaderID::Fraktal, "shader/fraktal/fraktal_vs.glsl","shader/fraktal/fraktal_fs.glsl");
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

	audioManager.PlaySong("sound/m4rc0-they-live.mp3");

	glfwSwapInterval(1); 

	glEnable(GL_DEPTH_TEST);

	{
		MarcelsTimer timer;
		SceneManager sceneManger;

		initRessource();
		
		// initial window registration
		viewportHandler.registerWithWindow(window);

		int width, height;
		glfwGetFramebufferSize(window, &width,&height);
		viewportHandler.framebufferSizeCallBack(width,height);

		// Projection and co
		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = camera.getViewMatrix();
		//glm::mat4* projection = viewportHandler.getOrthogonalProjection();
		glm::mat4& projectionOrthogonal = viewportHandler.getOrthogonalProjectionAddress();

		sceneManger.addScene(SceneFactory::createFraktalScene(maxHeight, maxWidth, projectionOrthogonal));

		while(!glfwWindowShouldClose(window)){
			delta = (float)timer.delta();

			processInput(window);

			audioManager.Update(delta);

			glClearStencil(0);
			glClear(GL_STENCIL_BUFFER_BIT);
			glClearColor(0.1f,0.1f,0.1f,1.0f);
			glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT| GL_STENCIL_BUFFER_BIT);
			
			sceneManger.update(delta);
			sceneManger.render();

			glfwSwapBuffers(window);
			glfwPollEvents();

			if(sceneManger.allFinished()){
				break;
			}
		}
	}
	
	glfwTerminate();

	audioManager.StopSongs();
	return 0;
}