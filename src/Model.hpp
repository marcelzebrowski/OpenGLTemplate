#ifndef MODEL_HPP
#define MODEL_HPP
#include <string>
#include <vector>
#include "Mesh.hpp"
#include "Shader.hpp"


class Model {
    public:
        Model(char* path){
            loadModel(path);
        }
        void draw(Shader& shader);

    private:
        std::vector<MeshRenderer::Mesh> meshes;
        void loadModel(std::string path);
};

#endif