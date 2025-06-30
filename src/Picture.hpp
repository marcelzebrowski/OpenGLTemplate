#ifndef PICTURE_HPP
#define PICTURE_HPP
#include "Shader.hpp"
#include "Texture.hpp"

class Picture {

private:

    unsigned int VAO, VBO, EBO;
    Shader* shader;
    Texture* texture;

    void setupMesh();

public:

    Picture(Shader* shader, Texture* texture);
    ~Picture();
    void render(float alpha, float elapsed,  glm::mat4 view, glm::mat4 model);

};

#endif