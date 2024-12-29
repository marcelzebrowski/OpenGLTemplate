#include "Cube.hpp"

Cube::Cube(Shader* shader, Texture* texture0, Texture* texture1)
    :shader(shader),texture0(texture0), texture1(texture1){

}

Cube::~Cube(){

}

void Cube::render(glm::mat4& model, glm::mat4& view){

}