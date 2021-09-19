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
using namespace std;

#define numVAOs 1
#define numVBOs 3

bool escapePressed = false;
float cameraX, cameraY, cameraZ;
float objLocX, objLocY, objLocZ;
GLuint renderingProgram;
GLuint vao[numVAOs];
GLuint vbo[numVBOs];
GLuint buntTexture;
//glm::vec3 initialLightLoc = glm::vec3(5.0f, 2.0f, 2.0f);
glm::vec3 initialLightLoc = glm::vec3(0.0f, 8.0f, 9.0f);

// variable allocation for display
GLuint mvLoc, projLoc, nLoc;

// variable allocation for material and light
GLuint globalAmbientLoc, lightAmbientLoc, materialAmbientLoc, materialDiffuseLoc, lightPositionLoc, lightDiffuseLoc, materialShininessLoc, lightSpecularLoc, materialSpecularLoc;

int width, height;
float aspect;
glm::mat4 pMat, vMat, mMat, mvMat, invTrMat;
glm::vec3 currentLightPosition, transformed;
float lightPosition[3];

ImportedModel myModel(GET_MODEL_PATH("rundeecken.obj"));

float toRadians(float degrees) { return (degrees * 2.0f * 3.14159f) / 360.0f; }

// light
float globalAmbient[4] = { 0.7f,0.7f,0.7f,1.0f };
float lightAmbient[4] = { 0.0f,0.0f,0.0f,1.0f };
float lightDiffuse[4] = { 1.0f,1.0f,1.0f,1.0f };
float lightSpecurlar[4] = { 1.0f,1.0f,1.0f,1.0f };


// material
float* materialAmbient = Utils::silverAmbient();
float* materialDiffuse = Utils::silverDiffuse();
float* materialSpecular = Utils::silverSpecular();
float materialShininess = Utils::silverShininess();

void setupLight(glm::mat4 vMatrix) {
	transformed = glm::vec3(vMatrix * glm::vec4(currentLightPosition, 1.0));

	lightPosition[0] = transformed.x;
	lightPosition[1] = transformed.y;
	lightPosition[2] = transformed.z;


	// get locations of the light and material fields in the shader
	globalAmbientLoc = glGetUniformLocation(renderingProgram, "globalAmbient");
	materialAmbientLoc = glGetUniformLocation(renderingProgram, "material.ambient");
	materialDiffuseLoc = glGetUniformLocation(renderingProgram, "material.diffuse");
	materialSpecularLoc = glGetUniformLocation(renderingProgram, "material.specular");
	lightAmbientLoc = glGetUniformLocation(renderingProgram, "light.ambient");
	lightDiffuseLoc = glGetUniformLocation(renderingProgram, "light.diffuse");
	lightPositionLoc = glGetUniformLocation(renderingProgram, "light.position");
	lightSpecularLoc = glGetUniformLocation(renderingProgram, "light.specular");
	materialShininessLoc = glGetUniformLocation(renderingProgram, "material.shininess");

	// set the uniform light and material values in the shader
	glProgramUniform4fv(renderingProgram, globalAmbientLoc, 1, globalAmbient);
	glProgramUniform4fv(renderingProgram, lightAmbientLoc, 1, lightAmbient);
	glProgramUniform4fv(renderingProgram, lightDiffuseLoc, 1, lightDiffuse);
	glProgramUniform4fv(renderingProgram, materialAmbientLoc, 1, materialAmbient);
	glProgramUniform4fv(renderingProgram, materialDiffuseLoc, 1, materialDiffuse);
	glProgramUniform3fv(renderingProgram, lightPositionLoc, 1, lightPosition);
	glProgramUniform4fv(renderingProgram, lightSpecularLoc, 1, lightSpecurlar);
	glProgramUniform4fv(renderingProgram, materialSpecularLoc, 1, materialSpecular);
	glProgramUniform1f(renderingProgram, materialShininessLoc, materialShininess);
}


void setupVertices(void) {

	std::vector<glm::vec3> vert = myModel.getVertices();
	std::vector<glm::vec2> tex = myModel.getTextureCoords();
	std::vector<glm::vec3> norm = myModel.getNormals();

	std::vector<float> pvalues;
	std::vector<float> tvalues;
	std::vector<float> nvalues;

	for (int i = 0; i < myModel.getNumVertices(); i++) {
		pvalues.push_back((vert[i]).x);
		pvalues.push_back((vert[i]).y);
		pvalues.push_back((vert[i]).z);


		if (myModel.isTextured()) {
			tvalues.push_back((tex[i]).s);
			tvalues.push_back((tex[i]).t);
		}

		nvalues.push_back((norm[i]).x);
		nvalues.push_back((norm[i]).y);
		nvalues.push_back((norm[i]).z);
	}

	glGenVertexArrays(1, vao);
	glBindVertexArray(vao[0]);
	glGenBuffers(numVBOs, vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, pvalues.size() * 4, &pvalues[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, nvalues.size() * 4, &nvalues[0], GL_STATIC_DRAW);


	if (myModel.isTextured()) {
		glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
		glBufferData(GL_ARRAY_BUFFER, tvalues.size() * 4, &tvalues[0], GL_STATIC_DRAW);
	}


}

void init(GLFWwindow* window) {
	renderingProgram = Utils::createShaderProgram(GET_SHADER_PATH("phong_vertShader.glsl"), GET_SHADER_PATH("phong_fragShader.glsl"));
	cameraX = 0.0f; cameraY = 0.0f; cameraZ = 5.0f;
	objLocX = 0.0f; objLocY = 0.0f; objLocZ = 0.0f;

	glfwGetFramebufferSize(window, &width, &height);
	aspect = (float)width / (float)height;
	pMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f);

	setupVertices();

	buntTexture = Utils::loadTexture(GET_TEXTUR_PATH("palette.png"));

}



void displayCube(GLFWwindow* window, double currentTime) {

	glUseProgram(renderingProgram);

	mvLoc = glGetUniformLocation(renderingProgram, "mv_matrix");
	projLoc = glGetUniformLocation(renderingProgram, "proj_matrix");
	nLoc = glGetUniformLocation(renderingProgram, "norm_matrix");

	vMat = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraX, -cameraY, -cameraZ));

	currentLightPosition = glm::vec3(initialLightLoc.x, initialLightLoc.y, initialLightLoc.z);

	// Object translating and rotating
	mMat = glm::translate(glm::mat4(1.0f), glm::vec3(objLocX, objLocY, objLocZ));
	mMat = glm::rotate(mMat, 0.0f, glm::vec3(15.0f, 0.0f, 0.0f));
	mMat = glm::rotate(mMat, toRadians(currentTime * 50), glm::vec3(0.0f, 1.0f, 1.0f));
	mMat = glm::rotate(mMat, toRadians(currentTime * 80), glm::vec3(-1.0f, 0.0f, 0.0f));

	setupLight(vMat);

	mvMat = vMat * mMat;

	invTrMat = glm::transpose(glm::inverse(mvMat));


	// set values to shader
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
	glUniformMatrix4fv(nLoc, 1, GL_FALSE, glm::value_ptr(invTrMat));

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);

	// Textur
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, buntTexture);




	glDrawArrays(GL_TRIANGLES, 0, myModel.getNumVertices());
}

void display(GLFWwindow* window, double currentTime) {
	glClear(GL_DEPTH_BUFFER_BIT);
	glClearColor(1.0, 0.5, 0.1, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	glEnable(GL_CULL_FACE);
	//glFrontFace(GL_CCW);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	displayCube(window, currentTime);
}

void window_size_callback(GLFWwindow* win, int newWidth, int newHeight) {
	aspect = (float)newWidth / (float)newHeight;
	glViewport(0, 0, newWidth, newHeight);
	pMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f);
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

	myMonitor = myMonitors[1]; // 0 wenn Main Monitor

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

	while (!glfwWindowShouldClose(window) && !escapePressed) {
		display(window, glfwGetTime());
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}