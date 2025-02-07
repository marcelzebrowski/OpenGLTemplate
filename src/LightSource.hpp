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
    glm::vec3 position;
    glm::vec3 color;
public:
    LightSource(Shader* shader);
    ~LightSource();

    void render(glm::mat4& model, glm::mat4& view);

    void setPosition(const glm::vec3 position);
    glm::vec3 getPosition() const;

    void setColor(const glm::vec3 color);
    glm::vec3 getColor() const;
};

#endif