#ifndef PICTURE_HPP
#define PICTURE_HPP
#include "Shader.hpp"
#include "Texture.hpp"

class Picture {

private:
    float alpha;
    float elapsed;
    glm::mat4* projection = nullptr;
    glm::mat4* view = nullptr;
    glm::mat4* model = nullptr;
    unsigned int VAO, VBO, EBO;
    Shader* shader;
    Texture* texture;

    void setupMesh();

public:

    Picture(Shader* shader, Texture* texture);
    ~Picture();
    void update(float alpha, float elapsed, glm::mat4* projection, glm::mat4* view, glm::mat4* model);
    void render();

};

#endif