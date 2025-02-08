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
	//maxWidth = 800;
	//maxHeight = 600;


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
		//Shader lightShader("shader/light/gouraud_vs.glsl","shader/light/gouraud_fs.glsl");
		Shader lightShader("shader/light/phong_vs.glsl","shader/light/phong_fs.glsl");
		Shader fraktalShader("shader/fraktal/fraktal_vs.glsl","shader/fraktal/fraktal_fs.glsl");

		Texture cube0Texture("texture/container2.png",0);
		Texture cube1Texture("texture/container2_specular.png",1);
		Texture emissionTexture("texture/matrix.jpg",2);


		setupLights(&lampShader);

		
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
		Cube cube(&lightShader,&camera, {&cube0Texture, &cube1Texture, &emissionTexture});

		LightSource lightSource(&lampShader);
		lightSource.setAmbientColor(glm::vec3(1.0f,1.0f,1.0f));

		// Fraktal
		Fraktal fraktal(&fraktalShader, height, width);

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

			fraktal.render(glfwGetTime());

			glm::mat4 model = glm::mat4(1.0f);
			glm::mat4 view = camera.getViewMatrix();

			coordinateSystem.render(model,view);

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

			glfwSwapBuffers(window);
			glfwPollEvents();
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}

	}
	
	glfwTerminate();

	return 0;
}
