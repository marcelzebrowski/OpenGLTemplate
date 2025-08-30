#include <iostream>
#include "Picture.hpp"
#include "utils/GLErrorCheck.hpp"

static float vertices[] = {
    // x, y, u, v
     0.0f,  0.0f,  0.0f, 0.0f,
     1.0f,  0.0f,  1.0f, 0.0f,
     1.0f,  1.0f,  1.0f, 1.0f,
     0.0f,  1.0f,  0.0f, 1.0f,
};

static unsigned int indices[]= {
    0,1,2, // first triangle
    0,2,3  // second triangle
};

Picture::Picture(Shader* shader, Texture* texture):shader(shader),texture(texture){
    setupMesh();
}

Picture::~Picture(){
    glDeleteVertexArrays(1,&VAO);
    glDeleteBuffers(1,&VBO);
    glDeleteBuffers(1,&EBO);
}

void Picture::update(float alpha, float elapsed, glm::mat4* projection, glm::mat4* view, glm::mat4* model){
    this->alpha = alpha;
    this->elapsed = elapsed;
    this->projection = projection;
    this->view = view;
    this->model = model;
}

void Picture::render(){

    // todo prüfen ob die ptr gesetzt sind

    if(view && model && projection){

        glDisable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        shader->attach();

            texture->attach(shader,"texture");
            shader->setFloat("alpha",alpha);
            shader->setMat4("view",*view);
            shader->setMat4("model",*model);
            shader->setMat4("projection",*projection);

        
            if(elapsed > 0){
                shader->setFloat("time",elapsed);
            }

            GL(glBindVertexArray(VAO));
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);

            texture->detach();
        shader->detach();
        glDisable(GL_BLEND);
        glEnable(GL_DEPTH_TEST);
    }
}

void Picture::setupMesh(){

    // create vertex and index buffer
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1,&EBO);

    // bin data
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // bind indices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // linking vertex attributes
    //                   index, size,  type,   normalized,  stride,          offset
    // position
    glVertexAttribPointer(0,2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // uv
    glVertexAttribPointer(1,2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2* sizeof(float)));
    glEnableVertexAttribArray(1);

    // unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}


