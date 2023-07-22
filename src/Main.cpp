#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <SOIL2/soil2.h>
#include <string>
#include <iostream>
#include <fstream>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include "ImportedModel.h"
#include "Utils.h"
#include "Torus.h"
#include "helpers/RootDir.h"
#include "AudioManager.h"
using namespace std;

bool escapePressed = false;
AudioManager sound;
const std::string music = GET_SOUND_PATH("CHROMAG - Switchback.mod");


float toRadians(float degrees) { return (degrees * 2.0f * 3.14159f) / 360.0f; }

void setupSound() {
	sound.LoadSong(music);
}

void stopSound() {
	sound.StopSFXs();
	sound.StopSongs();
}

void init(GLFWwindow* window) {
	setupSound();
}


void display(GLFWwindow* window, double currentTime) {
	glClear(GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	glEnable(GL_CULL_FACE);
	//glFrontFace(GL_CCW);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
}

void window_size_callback(GLFWwindow* win, int newWidth, int newHeight) {
	glViewport(0, 0, newWidth, newHeight);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		escapePressed = true;
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {

	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		escapePressed = true;
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
}

int main(void) {
	if (!glfwInit()) { exit(EXIT_FAILURE); }
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	GLFWmonitor* myMonitor = glfwGetPrimaryMonitor();

	int count = 0;
	GLFWmonitor** myMonitors = glfwGetMonitors(&count);

	myMonitor = myMonitors[0]; // 0 wenn Main Monitor

	const GLFWvidmode* mode = glfwGetVideoMode(myMonitor);

	GLFWwindow* window = glfwCreateWindow(mode->width, mode->height, "Template", myMonitor, NULL);


	glfwMakeContextCurrent(window);
	if (glewInit() != GLEW_OK) { exit(EXIT_FAILURE); }
	glfwSwapInterval(1);

	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetCursorPosCallback(window, cursor_position_callback);
	glfwSetWindowSizeCallback(window, window_size_callback);
	glfwSetKeyCallback(window, key_callback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

	init(window);

	sound.PlaySong(music);
	while (!glfwWindowShouldClose(window) && !escapePressed) {
		display(window, glfwGetTime());
		glfwSwapBuffers(window);
		glfwPollEvents();
		sound.Update(glfwGetTime());
	}

	glfwDestroyWindow(window);
	glfwTerminate();

	stopSound();
	exit(EXIT_SUCCESS);
}