#ifndef MESH_HP
#define MESH_HP

#include <glm/glm.hpp>
#include <vector>
#include <string>


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
        Mesh();
        ~Mesh();
    };

}
#endif