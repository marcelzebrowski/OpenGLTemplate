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

CoordinateSystem::CoordinateSystem(CoordinateSystem&& other) noexcept{
    VAO = other.VAO;
    VBO = other.VBO;
    EBO = other.EBO;

    other.VAO = 0;
    other.VBO = 0;
    other.EBO = 0;
}

CoordinateSystem& CoordinateSystem::operator=(CoordinateSystem&& other) noexcept{
    if(this != &other){
        this->~CoordinateSystem();
        VAO = other.VAO;
        VBO = other.VBO;
        EBO = other.EBO;

        other.EBO = 0;
        other.VAO = 0;
        other.EBO = 0;
    }
    return *this;
}

CoordinateSystem::CoordinateSystem(Shader* shader):shader(shader){
    
    GL(glGenVertexArrays(1,&VAO));
    GL(glGenBuffers(1,&VBO));
    GL(glGenBuffers(1,&EBO));

    GL(glBindVertexArray(VAO));
    GL(glBindBuffer(GL_ARRAY_BUFFER, VBO));
    GL(glBufferData(GL_ARRAY_BUFFER, sizeof(axesVertices),axesVertices, GL_STATIC_DRAW));

 
    // Überprüfe die Puffergröße
    GLint bufferSize = 0;
    glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &bufferSize);

    // Debug-Ausgabe
    std::cout << "Uploaded VBO size: " << bufferSize << " bytes" << std::endl;

    // Vergleich mit erwarteter Größe
    if (bufferSize != sizeof(axesVertices)) {
        std::cerr << "VBO size mismatch! Expected " << sizeof(axesVertices)
                << ", got " << bufferSize << " bytes." << std::endl;
    } else {
        std::cout << "VBO size matches expected size." << std::endl;
    }

    // create index buffer
    GL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO));
    GL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(axesIndices), axesIndices, GL_STATIC_DRAW));


    // link vertex attributes
    //                   index, size,  type,   normalized,  stride,          offset
	GL(glVertexAttribPointer(  0,  3    ,GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)0));// layout(location = 0) - Position
	GL(glVertexAttribPointer(  1,  3    ,GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(3*sizeof(float))));// layout(location = 1) - Color
	GL(glEnableVertexAttribArray(0)); 
	GL(glEnableVertexAttribArray(1));

    GL(glBindBuffer(GL_ARRAY_BUFFER,0));
    GL(glBindVertexArray(0));

    if (VAO == 0 || VBO == 0 || EBO == 0) {
        std::cerr << "OpenGL buffer creation failed!" << std::endl;
    }else{
        std::cout << "[DEBUG CoordinateSystem] VAO created: " << VAO << std::endl;
    }

    
}

CoordinateSystem::~CoordinateSystem(){
    glDeleteVertexArrays(1,&VAO);
    glDeleteBuffers(1,&VBO);
    glDeleteBuffers(1,&EBO);
}



void CoordinateSystem::update(const glm::mat4& projection, const glm::mat4& model, const glm::mat4& view){
    this->projection = projection;
    this->view = view;
    this->model = model;
}

void CoordinateSystem::render(){
    shader->attach();
        GL(glBindVertexArray(VAO));
        shader->setMat4("projection",projection);
        shader->setMat4("model",model);
        shader->setMat4("view",view);
        glDrawElements(GL_LINES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    shader->detach();
}
