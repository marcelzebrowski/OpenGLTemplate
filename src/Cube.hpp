#ifndef CUBE_HPP
#define CUBE_HPP

#include <glad/glad.h>
#include <glm/glm.hpp>
#include "Shader.hpp"
#include "Texture.hpp"
#include "GLErrorCheck.hpp"
#include <vector>
#include "Camera.hpp"

class Cube {
private:
    unsigned int VAO, VBO, EBO;
    Shader* shader;
    std::vector<Texture*> textures;
    Camera* camera;
public:

    Cube(Shader* shader, Camera* camera, const std::vector<Texture*> textures);
    ~Cube();

    void render(glm::mat4& model, glm::mat4& view, glm::vec3 lightPosition, float time);

};

#endif