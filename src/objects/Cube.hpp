#ifndef CUBE_HPP
#define CUBE_HPP

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>

#include "render/Shader.hpp"
#include "render/Texture.hpp"
#include "utils/GLErrorCheck.hpp"
#include "core/Camera.hpp"
#include "objects/LightSource.hpp"

class Cube {
private:
    unsigned int VAO, VBO, EBO;
    Shader* shader;
    std::vector<Texture*> textures;
    Camera* camera;
public:

    Cube(Shader* shader, Camera* camera, const std::vector<Texture*> textures);
    ~Cube();

    void render(glm::mat4& model, glm::mat4& view, const std::vector<LightSource*> lightSources, float time);

};

#endif