#include "Fraktal.hpp"

static float vertices[]{
    -1.0f, -1.0f,  1.0f, 
     1.0f, -1.0f,  1.0f, 
     1.0f,  1.0f,  1.0f, 
    -1.0f,  1.0f,  1.0f 
};

static unsigned int indices[]{
    0,1,2,0,2,3
};

Fraktal::Fraktal(Shader* shader):shader(shader){
    // create vertex and index buffer
    glGenVertexArrays(1,&VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // bind data
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // bind indices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // linking attributes
    glVertexAttribPointer(0,3,GL_FLOAT, GL_FALSE, 3*sizeof(float),(void*)0);
    glEnableVertexAttribArray(0);

    // unbind
    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindVertexArray(0);
}

Fraktal::~Fraktal(){
    glDeleteVertexArrays(1,&VAO);
    glDeleteBuffers(1,&VBO);
    glDeleteBuffers(1,&EBO);
}

void Fraktal::render(){
    shader->attach();
        GL(glBindVertexArray(VAO));
        glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
        glDrawElements(GL_TRIANGLES,2, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    shader->detach();

}