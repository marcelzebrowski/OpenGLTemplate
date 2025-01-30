#ifndef CUBE_HPP
#define CUBE_HPP

#include <glad/glad.h>
#include <glm/glm.hpp>
#include "Shader.hpp"
#include "Texture.hpp"
#include "GLErrorCheck.hpp"
#include <vector>

class Cube {
private:
    unsigned int VAO, VBO, EBO;
    Shader* shader;
    std::vector<Texture*> textures;

public:

    Cube(Shader* shader, const std::vector<Texture*> textures);
    ~Cube();

    void render(glm::mat4& model, glm::mat4& view, glm::vec3 lightPosition, glm::vec3 viewPosition, float time);

};

#endif