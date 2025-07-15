#include "D20Wireframe.hpp"
#include "GLErrorCheck.hpp"

D20Wireframe::D20Wireframe(Shader* shader):shader(shader){
    generateGeometry();
    setupMesh();
}

D20Wireframe::~D20Wireframe(){
    glDeleteBuffers(1,&VAO);
    glDeleteBuffers(1,&VBO);
    glDeleteBuffers(1,&EBO);
}

void D20Wireframe::setupMesh(){
    // create vertex and index buffer
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1,&EBO);

    // bin data
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);

    // bind indices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    // linking vertex attributes
    //                   index, size,  type,   normalized,  stride,          offset
    // position
    glVertexAttribPointer(0,3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    glEnableVertexAttribArray(0);

    // unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void D20Wireframe::generateGeometry(){

}

void D20Wireframe::update(float delta, float alpha, const glm::mat4* projection, const glm::mat4* view, const glm::mat4* model){
    this->alpha = alpha;
    this->model = model;
    this->projection = projection;
    this->view = view;
}

void D20Wireframe::render(){
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC1_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    shader->attach();
        shader->setMat4("view",*view);
        shader->setMat4("model",*model);
        shader->setMat4("projection",*projection);
        shader->setFloat("alpha",alpha);
        GL(glBindVertexArray(VAO));
        glDrawElements(GL_LINES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    shader->detach();
    glDisable(GL_BLEND);
}