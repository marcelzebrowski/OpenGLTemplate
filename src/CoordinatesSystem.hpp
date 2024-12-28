#ifndef COORDINATESSYSTEM_HPP
#define COORDINATESSYSTEM_HPP

#include <glad/glad.h>
#include <glm/glm.hpp>
#include "Shader.hpp"

class CoordinateSystem {
private:
    unsigned int VAO, VBO, EBO;
    Shader* shader;

public:
    CoordinateSystem(Shader* shader);
    ~CoordinateSystem();

    void render(glm::mat4& model, glm::mat4& view);

};

#endif