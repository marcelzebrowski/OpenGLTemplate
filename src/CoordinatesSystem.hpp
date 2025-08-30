#ifndef COORDINATESSYSTEM_HPP
#define COORDINATESSYSTEM_HPP

#include <glad/glad.h>
#include <glm/glm.hpp>
#include "Shader.hpp"
#include "utils/GLErrorCheck.hpp"

class CoordinateSystem {
private:
    unsigned int VAO, VBO, EBO;
    Shader* shader;
    glm::mat4* projection;
    glm::mat4* model;
    glm::mat4* view;

public:
    CoordinateSystem(Shader* shader);
    ~CoordinateSystem();

    CoordinateSystem(CoordinateSystem&& other) noexcept;
    CoordinateSystem& operator=(CoordinateSystem&& other) noexcept;

    CoordinateSystem(const CoordinateSystem&) = delete;
    CoordinateSystem& operator=(const CoordinateSystem&) = delete;

    void update(glm::mat4* projection, glm::mat4* model, glm::mat4* view);
    void render();

};

#endif