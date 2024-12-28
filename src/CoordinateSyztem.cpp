#include "CoordinatesSystem.hpp"

static const float axesVertices[]={
    // Positions + Colors
    0.0f,  0.0f,  0.0f,  0.0f, 1.0f, 0.0f, // Point 0 (X-Axis start)
    1.0f,  0.0f,  0.0f,  0.0f, 1.0f, 0.0f, // Point 1 (X-Axis end)
    0.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // Point 2 (Y-Axis start)
    0.0f,  1.0f,  0.0f,  1.0f, 0.0f, 0.0f, // Point 3 (Y-Axis end)
    0.0f,  0.0f,  0.0f,  0.0f, 0.0f, 1.0f, // Point 4 (Z-Axis start)
    0.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // Point 5 (Z-Axis end)
};

static const unsigned int axesIndices[] = {
    0, 1, // X-Axis
    2, 3, // Y-Axis
    4, 5, // Z-Axis
};


CoordinateSystem::CoordinateSystem(Shader* shader):shader(shader){
    
    // create index buffer
    glGenBuffers(1,&EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(axesIndices), axesIndices, GL_STATIC_DRAW);

    // create vertex array and vertex buffer object
    glGenVertexArrays(1,&VAO);
    glGenBuffers(1,&VBO);

    glBindVertexArray(VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(axesVertices),axesVertices, GL_STATIC_DRAW);

    // link vertex attributes
    //                   index, size,  type,   normalized,  stride,          offset
	glVertexAttribPointer(  0,  3    ,GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)0);// layout(location = 0) - Position
	glVertexAttribPointer(  1,  3    ,GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(3*sizeof(float)));// layout(location = 1) - Color
	glEnableVertexAttribArray(0); 
	glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindVertexArray(0);

}

CoordinateSystem::~CoordinateSystem(){
    glDeleteBuffers(1,&VAO);
    glDeleteBuffers(1,&VBO);
    glDeleteBuffers(1,&EBO);
}

void CoordinateSystem::render(glm::mat4& model, glm::mat4& view){
    shader->attach();
        shader->setMat4("model",model);
        shader->setMat4("view",view);
        glBindVertexArray(VAO);
        glDrawElements(GL_LINES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    shader->detach();
}
