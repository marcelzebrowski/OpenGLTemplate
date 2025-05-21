#include "Square.hpp"

static float vertices[]{
    -1.0f, -1.0f,  1.0f, 
     1.0f, -1.0f,  1.0f, 
     1.0f,  1.0f,  1.0f, 
    -1.0f,  1.0f,  1.0f 
};

static unsigned int indices[]{
    0,1,2,0,2,3
};

Square::Square(Shader* shader, const glm::vec3& color, const float scale):shader(shader),color(color),scale(scale){
    setup();
}

Square::~Square(){
    glDeleteVertexArrays(1,&VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void Square::setup(){

   glGenVertexArrays(1,&VAO);
   glGenBuffers(1,&VBO);
   glGenBuffers(1,&EBO);

   glBindVertexArray(VAO);
   glBindBuffer(GL_ARRAY_BUFFER,VBO);
   glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);

   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

   glVertexAttribPointer(0,3,GL_FLOAT, GL_FALSE, 3*sizeof(float),(void*)0);
   glEnableVertexAttribArray(0);

   glBindBuffer(GL_ARRAY_BUFFER,0);
   glBindVertexArray(0);
}


void Square::render(){
    glDisable(GL_DEPTH_TEST);
    shader->attach();
        GL(glBindVertexArray(VAO));
        shader->setFloat3("color",color);
        shader->setFloat("scale",scale);
        glDrawElements(GL_TRIANGLES,6, GL_UNSIGNED_INT,0);
        glBindVertexArray(0);

    shader->detach();
    glEnable(GL_DEPTH_TEST);
}

void Square::setColor(glm::vec3& color){
    this->color = color;
}
    

void Square::setScale(float scale){
    this->scale = scale;
}