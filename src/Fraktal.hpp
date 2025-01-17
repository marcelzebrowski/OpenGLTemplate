#ifndef FRAKTAL_H
#define FRAKTAL_H
#include <glad/glad.h>
#include <glm/glm.hpp>
#include "Shader.hpp"
#include "GLErrorCheck.hpp"
class Fraktal{
private:
    unsigned int VAO, VBO, EBO;
    Shader* shader;

public:
    Fraktal(Shader* shader);
    ~Fraktal();
    void render();
};

#endif