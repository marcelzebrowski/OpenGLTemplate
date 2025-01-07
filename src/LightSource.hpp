#ifndef LIGHTSOURCE_HPP
#define LIGHTSOURCE_HPP

#include <glad/glad.h>
#include <glm/glm.hpp>
#include "Shader.hpp"
#include "GLErrorCheck.hpp"

class LightSource {
private:
    unsigned int VAO, VBO, EBO;
    Shader* shader;
public:
    LightSource(Shader* shader);
    ~LightSource();

    void render(glm::mat4& model, glm::mat4& view);
};

#endif