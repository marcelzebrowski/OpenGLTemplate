#ifndef FRAKTAL_H
#define FRAKTAL_H
#include <glad/glad.h>
#include <glm/glm.hpp>
#include "Shader.hpp"
#include "GLErrorCheck.hpp"

class Fraktal {
private:
    unsigned int VAO, VBO, EBO;
    Shader* shader;
    float zoom;
    float centerX;
    float centerY;
    int height;
    int width;
    float delta;

public:
    Fraktal(Shader* shader, int height, int width);
    ~Fraktal();
    void update(float delta);
    void render();
};

#endif