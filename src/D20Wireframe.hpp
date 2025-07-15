#ifndef D20WIREFRAME_HPP
#define D20WIREFRAME_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include "Shader.hpp"

class D20Wireframe {

    public:
        D20Wireframe(Shader* shader);
        ~D20Wireframe();
        void update(float delta, float alpha, const glm::mat4* projection, const glm::mat4* view, const glm::mat4* model);
        void render();

    private:
        Shader* shader;
        const glm::mat4* projection = nullptr;
        const glm::mat4* view = nullptr;
        const glm::mat4* model = nullptr;
        std::vector<glm::vec3> vertices;
        std::vector<unsigned int> indices;
        float rotationSpeed = 0.5f;
        float angle = 0.0f;
        float alpha;
        unsigned int VAO = 0, VBO = 0, EBO = 0;
        void setupMesh();
        void generateGeometry();

};
#endif