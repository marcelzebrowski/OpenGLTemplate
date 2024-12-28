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



// Generiere den Torus
unsigned int createTorus(float R, float r, unsigned int numTheta, unsigned int numPhi) {
    std::vector<float> vertices; // Vektor, um alle Vertices zu speichern
    std::vector<unsigned int> indices; // Vektor für die Indices

    // Winkel für Theta und Phi
    float thetaStep = 2.0f * (float)M_PI / numTheta;
    float phiStep = 2.0f * (float)M_PI / numPhi;

    // Generiere alle Vertices
    for (unsigned int i = 0; i < numTheta; ++i) {
        for (unsigned int j = 0; j < numPhi; ++j) {
            float theta = i * thetaStep;
            float phi = j * phiStep;

            // Position des Punktes auf dem Torus
            float x = (R + r * cos(phi)) * cos(theta);
            float y = (R + r * cos(phi)) * sin(theta);
            float z = r * sin(phi);

            // Texturkoordinaten
            float u = (float)i / (numTheta - 1);
            float v = (float)j / (numPhi - 1);

            // Farbe (hier zufällig, könnte angepasst werden)
            float red = (sin(theta) + 1.0f) / 2.0f;
            float green = (cos(theta) + 1.0f) / 2.0f;
            float blue = (sin(phi) + 1.0f) / 2.0f;

            // Füge die Vertex-Daten hinzu (Position + Farbe + Texturkoordinaten)
            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);
            vertices.push_back(red);
            vertices.push_back(green);
            vertices.push_back(blue);
            vertices.push_back(u);
            vertices.push_back(v);
        }
    }

    // Generiere die Indices für die Dreiecke (Kombination der Vertices)
    for (unsigned int i = 0; i < numTheta; ++i) {
        for (unsigned int j = 0; j < numPhi; ++j) {
            unsigned int first = (i * numPhi + j) % (numTheta * numPhi);
            unsigned int second = (first + numPhi) % (numTheta * numPhi);
            unsigned int third = (first + 1) % (numTheta * numPhi);
            unsigned int fourth = (second + 1) % (numTheta * numPhi);

            // Erstelle zwei Dreiecke für jedes Rechteck auf dem Torus
            indices.push_back(first);
            indices.push_back(second);
            indices.push_back(third);

            indices.push_back(third);
            indices.push_back(second);
            indices.push_back(fourth);
        }
    }

    // VAO, VBO und EBO erstellen
    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // Binde VAO
    glBindVertexArray(VAO);

    // Binde VBO und lade die Vertices
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    // Binde EBO und lade die Indices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    // Definiere Vertex-Attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0); // Position
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float))); // Farbe
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float))); // Texturkoordinaten
    glEnableVertexAttribArray(2);

    // Unbind VAO
    glBindVertexArray(0);

    return VAO;
}


unsigned int createCube(){
	// Vertices für den Würfel mit 6 Seiten
    float vertices[] = {
        // Position           // Farbe           // Texturkoordinaten
        // Vorderseite
        -0.5f, -0.5f,  0.5f,   1.0f, 0.0f, 0.0f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,   0.0f, 1.0f, 0.0f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,   0.0f, 0.0f, 1.0f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,   1.0f, 1.0f, 0.0f,  0.0f, 1.0f,

        // Rückseite
        -0.5f, -0.5f, -0.5f,   1.0f, 0.0f, 1.0f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,   0.0f, 1.0f, 1.0f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,   1.0f, 1.0f, 0.0f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,   0.0f, 0.0f, 1.0f,  0.0f, 1.0f,

        // Linke Seite
        -0.5f, -0.5f, -0.5f,   1.0f, 0.5f, 0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,   0.5f, 1.0f, 0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,   0.0f, 0.5f, 1.0f,  1.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,   0.5f, 0.0f, 1.0f,  0.0f, 1.0f,

        // Rechte Seite
         0.5f, -0.5f, -0.5f,   1.0f, 0.0f, 0.5f,  0.0f, 0.0f,
         0.5f,  0.5f, -0.5f,   0.5f, 1.0f, 0.0f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,   1.0f, 0.5f, 1.0f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,   0.0f, 0.0f, 1.0f,  0.0f, 1.0f,

        // Unterseite
        -0.5f, -0.5f, -0.5f,   1.0f, 1.0f, 0.0f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,   0.0f, 1.0f, 0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,   0.5f, 0.5f, 1.0f,  1.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,   0.0f, 0.0f, 0.5f,  0.0f, 1.0f,

        // Oberseite
        -0.5f,  0.5f, -0.5f,   1.0f, 0.0f, 0.0f,  0.0f, 0.0f,
         0.5f,  0.5f, -0.5f,   0.0f, 1.0f, 0.0f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,   0.0f, 0.0f, 1.0f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,   1.0f, 1.0f, 0.0f,  0.0f, 1.0f,
    };

    // Indizes für den Würfel
    unsigned int indices[] = {
        // Vorderseite
        0, 1, 2,   0, 2, 3,
        // Rückseite
        4, 5, 6,   4, 6, 7,
        // Linke Seite
        8, 9, 10,  8, 10, 11,
        // Rechte Seite
        12, 13, 14, 12, 14, 15,
        // Unterseite
        16, 17, 18, 16, 18, 19,
        // Oberseite
        20, 21, 22, 20, 22, 23,
    };


	// create vertex and index buffer
	unsigned int VAO, VBO, EBO;
	glGenVertexArrays(1,&VAO);
	glGenBuffers(1,&VBO);
	glGenBuffers(1, &EBO);
	
	// bind data
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// bind indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	
	// -- linking vertex attributes (interpret the vertex data)

	//                   index, size,  type,   normalized,  stride,          offset
	// Vertex-Attribute definieren
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0); // Position
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float))); // Farbe
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float))); // Texturkoordinaten
    glEnableVertexAttribArray(2);

	// -- unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

	return VAO;
}

unsigned int createTriangle(){
	float vertices[] = {
		// positions       +     color       +    texture cords
		 0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		 0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
		-0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
		-0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f  
	};

	unsigned int indeces[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};


	// create vertex and index buffer
	unsigned int VAO, VBO, EBO;
	glGenVertexArrays(1,&VAO);
	glGenBuffers(1,&VBO);
	glGenBuffers(1, &EBO);
	
	// bind data
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// bind indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indeces), indeces, GL_STATIC_DRAW);
	
	// -- linking vertex attributes (interpret the vertex data)

	//                   index, size,  type,   normalized,  stride,          offset
	glVertexAttribPointer(  0,  3    ,GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)0);// layout(location = 0)
	glVertexAttribPointer(  1,  3    ,GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(3*sizeof(float)));// layout(location = 1)
	glVertexAttribPointer(  2,  2    ,GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(6*sizeof(float)));// layout(location = 2)
	glEnableVertexAttribArray(0); 
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2); 

	// -- unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

	return VAO;
}




float randomColor(){
	float timeValue = (float)glfwGetTime();
	float value = (sin(timeValue*doRandom())/2.0f) + 0.5f;
	return value;
}

void render(const unsigned int VAO){
	glBindVertexArray(VAO);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	//glDrawElements(GL_TRIANGLES, 6 * 100 * 100, GL_UNSIGNED_INT, 0);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
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
		Shader shader("shader/vertex.glsl","shader/fragment.glsl");
		ViewportHandler viewportHandler;
		viewportHandler.addShader(&shader);

		// initial window registration
		viewportHandler.registerWithWindow(window);

		int width, height;
		glfwGetFramebufferSize(window, &width,&height);
		viewportHandler.framebufferSizeCallBack(width,height);

	
		Texture containerTexture("texture/container.png",0);
		Texture awesomeFaceTexture("texture/awesomeface.png",1);


		unsigned int VAO = createCube();
		
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

			glClearColor(0.2f,0.3f,0.3f,1.0f);
			glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

			

			shader.attach();
			containerTexture.attach(shader,"texture1");
			awesomeFaceTexture.attach(shader,"texture2");

			// create view (camera)
			glm::mat4 view = glm::mat4(1.0f);
			view = glm::translate(view, glm::vec3(0.0f,0.0f,-3.0f));
			shader.setMat4("view",view);
			
			for(unsigned int i=0; i<10;i++){
				
				float c1 = randomColor();
				float c2 = randomColor();
				float c3 = randomColor();

				// create model
				glm::mat4 model = glm::mat4(1.0f);
				model = glm::translate(model,cubePositions[i]);
				model = glm::rotate(model, (float)glfwGetTime(),cubePositions[i]);
				
				shader.setFloat3("color",c1,c2,c3);
				shader.setMat4("model",model);
				
				render(VAO);

				
			}

			awesomeFaceTexture.detach();
			containerTexture.detach();
			shader.detach();

			

			
			glfwSwapBuffers(window);
			glfwPollEvents();    
		}

		// clean
		glDeleteVertexArrays(1,&VAO);
	}
	
	glfwTerminate();

	return 0;
}
