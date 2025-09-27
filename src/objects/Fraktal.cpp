#include "objects/Fraktal.hpp"

static float vertices[]{
    -1.0f, -1.0f,  1.0f, 
     1.0f, -1.0f,  1.0f, 
     1.0f,  1.0f,  1.0f, 
    -1.0f,  1.0f,  1.0f 
};

static unsigned int indices[]{
    0,1,2,0,2,3
};

Fraktal::Fraktal(Shader& shader, int height, int width)
    :shader(shader),zoom(1.0f),centerX(-0.743643887037151f), centerY(0.13182590420533f), height(height), width(width){
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

void Fraktal::update(float delta){
    zoom = 1.0f * delta;
    centerX += sin(delta) * 0.0002f;
    centerY += cos(delta) * 0.0002f;
}

void Fraktal::render(float alpha){
    
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    shader.attach();

        shader.setFloat2("uResolution",(float)width, (float)height);
        shader.setFloat2("uCenter",centerX,centerY);
        shader.setFloat("uZoom", zoom);
        shader.setFloat("uTime",zoom);
        shader.setFloat("uAlpha",alpha);

        glBindVertexArray(VAO);
        glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
        glDrawElements(GL_TRIANGLES,6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    shader.detach();
    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
}