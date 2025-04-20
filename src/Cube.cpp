#include "Cube.hpp"

static float vertices[] = {
    // Position           // Normalenvektoren         // Texturkoordinaten
    // Vorderseite
    -0.5f, -0.5f,  0.5f,   0.0f,  0.0f,  1.0f,   0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,   0.0f,  0.0f,  1.0f,   1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,   0.0f,  0.0f,  1.0f,   1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,   0.0f,  0.0f,  1.0f,   0.0f, 1.0f,

    // Rückseite
    -0.5f, -0.5f, -0.5f,   0.0f,  0.0f, -1.0f,   0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,   0.0f,  0.0f, -1.0f,   1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,   0.0f,  0.0f, -1.0f,   1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,   0.0f,  0.0f, -1.0f,   0.0f, 1.0f,

    // Linke Seite
    -0.5f, -0.5f, -0.5f,  -1.0f,  0.0f,  0.0f,   0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  -1.0f,  0.0f,  0.0f,   1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  -1.0f,  0.0f,  0.0f,   1.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  -1.0f,  0.0f,  0.0f,   0.0f, 1.0f,

    // Rechte Seite
     0.5f, -0.5f, -0.5f,   1.0f,  0.0f,  0.0f,   0.0f, 0.0f,
     0.5f,  0.5f, -0.5f,   1.0f,  0.0f,  0.0f,   1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,   1.0f,  0.0f,  0.0f,   1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,   1.0f,  0.0f,  0.0f,   0.0f, 1.0f,

    // Unterseite
    -0.5f, -0.5f, -0.5f,   0.0f, -1.0f,  0.0f,   0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,   0.0f, -1.0f,  0.0f,   1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,   0.0f, -1.0f,  0.0f,   1.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,   0.0f, -1.0f,  0.0f,   0.0f, 1.0f,

    // Oberseite
    -0.5f,  0.5f, -0.5f,   0.0f,  1.0f,  0.0f,   0.0f, 0.0f,
     0.5f,  0.5f, -0.5f,   0.0f,  1.0f,  0.0f,   1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,   0.0f,  1.0f,  0.0f,   1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,   0.0f,  1.0f,  0.0f,   0.0f, 1.0f,
};

    // Indizes für den Würfel
    static unsigned int indices[] = {
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

Cube::Cube(Shader* shader, Camera* camera, const std::vector<Texture*> textures)
    :shader(shader),camera(camera),textures(textures){
    // create vertex and index buffer
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

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float))); // Normal
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float))); // Texturkoordinaten
    glEnableVertexAttribArray(2);

	// -- unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

Cube::~Cube(){
    glDeleteVertexArrays(1,&VAO);
    glDeleteBuffers(1,&VBO);
    glDeleteBuffers(1,&EBO);
}

void Cube::render(glm::mat4& model, glm::mat4& view, const std::vector<LightSource*> lightSources, float time){
    shader->attach();

        textures[0]->attach(shader,"material.diffuse");
        textures[1]->attach(shader,"material.specular");

        
        shader->setMat4("model",model);
        shader->setMat4("view",view);
        shader->setFloat3("viewPosition", camera->getPosition());
        shader->setFloat("material.shininess",32.0f);

        // directional light
        shader->setFloat3("directionalLight.direction",-0.2f,-1.0f,-0.3f);
        shader->setFloat3("directionalLight.ambient",0.2f,0.2f,0.2f);
        shader->setFloat3("directionalLight.diffuse",0.5f,0.5f,0.5f);
        shader->setFloat3("directionalLight.specular",1.0f,1.0f,1.0f);

        // point light
        // Setze die Point Lights aus dem Parameter
        for (size_t i = 0; i < lightSources.size(); ++i) {
            std::string index = std::to_string(i);
            shader->setFloat3("pointLight[" + index + "].position", lightSources[i]->getPosition());
            shader->setFloat("pointLight[" + index + "].constant", 1.0f);
            shader->setFloat("pointLight[" + index + "].linear", 0.09f);
            shader->setFloat("pointLight[" + index + "].quadratic", 0.032f);
            shader->setFloat3("pointLight["+ index+ "].ambient", lightSources[i]->getAmbientColor());
            shader->setFloat3("pointLight[" + index + "].diffuse", lightSources[i]->getDiffuseColor());
            shader->setFloat3("pointLight["+ index + "].specular", lightSources[i]->getSpecularColor());
        }

        //spotlight
        shader->setFloat3("spotLight.position",camera->getPosition());
        shader->setFloat3("spotLight.direction",camera->getTarget());
        shader->setFloat3("spotLight.ambient", 0.05f, 0.0f, 0.0f);
        shader->setFloat3("spotLight.diffuse", 0.5f, 0.0f, 0.0f);
        shader->setFloat3("spotLight.specular", 1.0f, 0.0f, 0.0f);

        shader->setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
        shader->setFloat("spotLight.outerCutOff", glm::cos(glm::radians(14.5f)));
        shader->setFloat("spotLight.constant", 1.0f);
        shader->setFloat("spotLight.linear", 0.09f);
        shader->setFloat("spotLight.quadratic", 0.032f);


        

        GL(glBindVertexArray(VAO));
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);


        textures[2]->detach();
        textures[1]->detach();
        textures[0]->detach();
    shader->detach();

}

