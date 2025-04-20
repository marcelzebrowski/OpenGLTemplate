#ifndef MESH_HP
#define MESH_HP

#include <glm/glm.hpp>
#include <vector>
#include <string>
#include "Shader.hpp"

namespace MeshRenderer {

    struct Vertex {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 texCoords;
    };

    struct Texture {
        unsigned int id;
        std::string type;
        std::string path;
    };

    class Mesh {
    public:
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<Texture> textures;

        Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
        void Draw(Shader *shader);
        ~Mesh();
        
    private:
        unsigned int VAO, VBO, EBO;
        void setupMesh();

        
    };

}
#endif