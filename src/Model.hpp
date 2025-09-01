#ifndef MODEL_HPP
#define MODEL_HPP
#include <string>
#include <vector>

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"



#include "render/Mesh.hpp"
#include "render/Shader.hpp"



class Model {
    public:
        Model(std::string const &path, Shader* shader):shader(shader){
            loadModel(path);
        }
        void render(glm::mat4& model, glm::mat4& view);

    private:
        std::vector<MeshRenderer::Mesh> meshes;
        std::string directory;
        std::vector<MeshRenderer::Texture> texturesLoaded;
        Shader* shader;
        
        void loadModel(std::string path);

        void processNode(aiNode* node, const aiScene* scene);
        MeshRenderer::Mesh processMesh(aiMesh* mesh, const aiScene* scene);
        std::vector<MeshRenderer::Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
        unsigned int loadTextureFromDisc(const char* filePath);
};

#endif