#ifndef CUBE_HPP
#define CUBE_HPP

#include <glad/glad.h>
#include <glm/glm.hpp>
#include "Shader.hpp"

class Cube {
private:
    unsigned int VAO, VBO, EBO;
    Shader* shader;

public:
    Cube(Shader* shader);
    ~Cube();

    void render(glm::mat4& model, glm::mat4& view);

};

#endif