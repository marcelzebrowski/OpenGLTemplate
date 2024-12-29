#ifndef CUBE_HPP
#define CUBE_HPP

#include <glad/glad.h>
#include <glm/glm.hpp>
#include "Shader.hpp"
#include "Texture.hpp"
#include "GLErrorCheck.hpp"

class Cube {
private:
    unsigned int VAO, VBO, EBO;
    Shader* shader;
    Texture* texture0;
    Texture* texture1;

public:
    Cube(Shader* shader, Texture* texture0, Texture* texture1);
    ~Cube();

    void render(glm::mat4& model, glm::mat4& view,float c1, float c2, float c3);

};

#endif