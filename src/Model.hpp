#ifndef MODEL_HPP
#define MODEL_HPP
#include <string>
#include <vector>

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Mesh.hpp"
#include "Shader.hpp"
#include "Texture.hpp"


class Model {
    public:
        Model(char* path){
            loadModel(path);
        }
        void draw(Shader& shader);

    private:
        Texture textureHelper;
        std::vector<MeshRenderer::Mesh> meshes;
        std::string directory;
        
        void loadModel(std::string path);

        void processNode(aiNode* node, const aiScene* scene);
        MeshRenderer::Mesh processMesh(aiMesh* mesh, const aiScene* scene);
        std::vector<MeshRenderer::Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
};

#endif