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
		window = glfwCreateWindow(maxWidth, maxHeight, "Nerdvana 2025", NULL, NULL);
	#else
		// Release-Modus: Fullscreen
		window = glfwCreateWindow(maxWidth, maxHeight, "Nerdvana 2025", monitor, NULL);
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
	audioManager.PlaySong("sound/embrace-364091.mp3");


	glfwSwapInterval(1); 

	glEnable(GL_DEPTH_TEST);

	{
		const char* rawText = 
								"Jeah,... heute komme ich mit was ganz Besonderem daher. Zur Zeit habe ich Urlaub und etwas Zeit diese so zu verplempern. "
								"So wie es sich in einem Intro gehoert erst einmal ein paar Gruesse ... "
								"Ich gehe einfach die Liste aus dem Discord durch – die gerade online sind, kommen als erstes: "
								"EinfachNurA und Praiot (ich) ... hm, das war's schon. "
								"Dann gruesse ich noch die Wuerfelbots D1-C3, Midjourney Bot und Wuefelbot – "
								"ich glaube, ich werde die mal entfernen, die benutzt eh keiner. "
								"So, und dann noch alle, die offline sind: Azradamus, Crash – the one and only, "
								"DerPatze, DirtyLittleDice, Drizzt1981, Gerowinger, Hodentod, Kaiwalker, "
								"Koenig Donnerdarm von Discordia, Martin, Nawami, NuvOk, Orakel, Pukis, Razoreth, Reg, SirPadras und Triback "
								"(Goldenes Camel). "
								"... so, es ist nun der 16.7.2025 und schon etwas spaet, 18:52 Uhr – "
								"und der dicke alte Onkel muesste eigentlich ins Fittnessstudio gehen, um Fett abzubauen. Aber stattdessen bastelt er an einem Intro rum. :-D "
								"Naechstes Jahr mal auf einer Demoparty auftauchen, wird zwar keine Schnitte haben, aber Intro geht immer ;-). Bis dahin will ich aber noch eine Art "
								"Scenen Graph implementieren, damit man nicht immer das selbe sehen muss. Aber bis Ostern 2026 ist noch etwas Zeit. So jetzt aber, bis hier liest eh keiner, macht es gut. "
								"Der Text wiederholt sich nun.";
								std::string flatText(rawText);




		MarcelsTimer timer;

		Shader fraktalShader("shader/fraktal/fraktal_vs.glsl","shader/fraktal/fraktal_fs.glsl");
		Shader coordinateSystemShader("shader/axes/axes_vertex.glsl","shader/axes/axes_fragment.glsl");
		Shader pictureShader("shader/picture/vertex.glsl","shader/picture/fragment.glsl");
		Shader pictureWobbleShader("shader/picture/vertex.glsl","shader/picture/fragment_wobbel.glsl");
		Shader textShader("shader/text/vertex.glsl","shader/text/fragment.glsl");
		Shader d20Shader("shader/d20Wireframe/vertex.glsl", "shader/d20Wireframe/fragment.glsl");

		// initial window registration
		viewportHandler.registerWithWindow(window);

		int width, height;
		glfwGetFramebufferSize(window, &width,&height);
		viewportHandler.framebufferSizeCallBack(width,height);

	
		CoordinateSystem coordinateSystem(&coordinateSystemShader);
	

		std::vector<std::unique_ptr<Texture>> textures;
		
		textures.emplace_back(std::make_unique<Texture>("texture/umbreon.png",0));
		textures.emplace_back(std::make_unique<Texture>("texture/koali.png",0));
		textures.emplace_back(std::make_unique<Texture>("texture/loeffel.png",0));
		textures.emplace_back(std::make_unique<Texture>("texture/clawdeen.png",0));
		textures.emplace_back(std::make_unique<Texture>("texture/tsu.png",0));
		textures.emplace_back(std::make_unique<Texture>("texture/frank.png",0));
		textures.emplace_back(std::make_unique<Texture>("texture/anguyx.png",0));
		textures.emplace_back(std::make_unique<Texture>("texture/police.png",0));
		textures.emplace_back(std::make_unique<Texture>("texture/elfe.png",0));
		textures.emplace_back(std::make_unique<Texture>("texture/indianer.png",0));

		std::vector<PictureFadeController> fadeControllers;
		std::vector<PictureAnimator> animators;
		std::vector<std::unique_ptr<Picture>> pictures;

		for(auto& texture : textures){
			Picture picture(&pictureShader, texture.get());
			pictures.emplace_back(std::make_unique<Picture>(&pictureShader, texture.get()));
		}

		for(auto& picture : pictures){
			fadeControllers.emplace_back(picture.get());
		}

		for(auto& controller : fadeControllers){
			PictureAnimator pictureAnimator(1100.0f,1536.0f,5.0f, &controller);
			pictureAnimator.setTargetSize((float)maxWidth, (float)maxHeight);
			animators.emplace_back(pictureAnimator);
		}

		PictureAnimatorManager pictureAnimatorManager(animators,30.0f);

		Texture pictureNerdvana("texture/nerdvana4.png",0);
		Texture textTexture("texture/ASCII.png",0);

		Picture logo (&pictureWobbleShader, &pictureNerdvana);

		PictureFadeController logoFadeController(&logo);

		PictureAnimator logoAnimator(1024.0f,1024.0f,10.0f, &logoFadeController,AnimationType::Swing);
		logoAnimator.setTargetSize(maxWidth/2.0f, maxHeight/2.0f);
		logoAnimator.start();


		Fraktal fraktal(&fraktalShader, maxHeight, maxWidth);

		D20Wireframe d20Wireframe(&d20Shader);
		D20WireframeAnimator d20WireframeAnimator(&d20Wireframe, aspectRatio);


		Text text(&textShader,&textTexture);

		float frak = 0.0f;

		ScrollingText scrollingText(&text, flatText, (float)maxWidth, (float) maxHeight);
		scrollingText.setStartDelay(5.0f);
		scrollingText.setSpeed(320.0f);
		scrollingText.setAmplitude(100.0f);
		scrollingText.setFrequency(0.5f);
		scrollingText.setBasePosition(glm::vec2(0.0f,300.0f));
		scrollingText.setScale(2500.0f);
	
		//std::this_thread::sleep_for(std::chrono::seconds(10));
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

			frak += delta;
			fraktal.update(frak);
			fraktal.render();

			pictureAnimatorManager.update(delta, viewportHandler.getOrthogonalProjection());
			pictureAnimatorManager.render();

			logoAnimator.update(delta, viewportHandler.getOrthogonalProjection());
			logoAnimator.render();

			// d20
			d20WireframeAnimator.update(delta, viewportHandler.getProjection());
			d20WireframeAnimator.render();

			scrollingText.update(delta, viewportHandler.getOrthogonalProjection());
			scrollingText.render();

			// coordinate
			//coordinateSystem.update(viewportHandler.getProjection(), &model, &view);
			//coordinateSystem.render();

		
			glfwSwapBuffers(window);
			glfwPollEvents();
			//std::this_thread::sleep_for(std::chrono::milliseconds(1));
			
		}

	}
	
	glfwTerminate();

	audioManager.StopSongs();

	return 0;
}