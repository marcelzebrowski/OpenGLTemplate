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
    glm::vec3 diffuseColor;
    glm::vec3 ambientColor;
    glm::vec3 specularColor;
public:
    LightSource(Shader* shader);
    ~LightSource();

    void render(glm::mat4& model, glm::mat4& view);

    void setPosition(const glm::vec3 position);
    glm::vec3 getPosition() const;

    void setDiffuseColor(const glm::vec3 color);
    glm::vec3 getDiffuseColor() const;

    void setAmbientColor(const glm::vec3 color);
    glm::vec3 getAmbientColor() const;

    void setSpecularColor(const glm::vec3 color);
    glm::vec3 getSpecularColor() const;
};

#endif