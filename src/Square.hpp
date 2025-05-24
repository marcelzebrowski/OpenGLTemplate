#ifndef SQUARE_HPP
#define SQUARE_HPP

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "Shader.hpp"
#include "GLErrorCheck.hpp"


class Square{
private:
    unsigned int VAO, VBO, EBO;
    Shader* shader;
    glm::vec3 color;
    float scale;

    void setup();

public:
    Square(Shader* shader, const glm::vec3& color = glm::vec3(1.0f), const float scale = 1.0f);
    ~Square();

    void render(glm::mat4& model, glm::mat4& view);
    void setColor(glm::vec3& color);
    void setScale(float scale);

};

#endif