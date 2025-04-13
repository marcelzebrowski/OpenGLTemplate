#include "Model.hpp"



void Model::draw(Shader& shader) {
    for (size_t i = 0; i < meshes.size(); i++) {
        meshes[i].Draw(shader);
    }
}

void Model::loadModel(std::string path){
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate| aiProcess_FlipUVs);
    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
        return;
    }
    directory = path.substr(0, path.find_last_of('/'));

    processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene){
    // process all the meshes in this node
    for(unsigned int i = 0; i < node->mNumMeshes; i++){
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene));
    }

    // then do the same for each of its children
    for(unsigned int i = 0; i < node->mNumChildren; i++){
        processNode(node->mChildren[i], scene);
    }
}

MeshRenderer::Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene){
    std::vector<MeshRenderer::Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<MeshRenderer::Texture> textures;

    for(unsigned int i=0; i < mesh->mNumVertices; i++){
        MeshRenderer::Vertex vertex;
        // process all the vertex attributes (position, normal, texCoords)
        glm::vec3 vector;
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.position = vector;

        vector.x = mesh->mNormals[i].x;
        vector.y = mesh->mNormals[i].y;
        vector.z = mesh->mNormals[i].z;
        vertex.normal = vector;

        if(mesh->mTextureCoords[0]){ // does the mesh contain texture coordinates?
            glm::vec2 vec;
            vec.x = mesh->mTextureCoords[0][i].x; // a texture coordinate
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.texCoords = vec;
        }else{
            vertex.texCoords = glm::vec2(0.0f, 0.0f); // if not, assign a default value (0, 0)
        }

        vertices.push_back(vertex);
    }

    // process indices
    for(unsigned int i = 0; i < mesh->mNumFaces; i++){
        aiFace face = mesh->mFaces[i];
        for(unsigned int j=0; j<face.mNumIndices; j++){
            indices.push_back(face.mIndices[j]);
        }
    }

    // process material
    if(mesh->mMaterialIndex >= 0){
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        std::vector<MeshRenderer::Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE,"texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        std::vector<MeshRenderer::Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    }

    return MeshRenderer::Mesh(vertices, indices, textures);
}

std::vector<MeshRenderer::Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName){
    std::vector<MeshRenderer::Texture> textures;
    for(unsigned int i = 0; i < mat->GetTextureCount(type); i++){
        aiString fileName;
        mat->GetTexture(type, i, &fileName);
        MeshRenderer::Texture texture;
        std::string fullPath = directory + "/" + fileName.C_Str();
        texture.id = textureHelper.createTexture(fullPath.c_str());
        texture.type = typeName;
        texture.path = fileName.C_Str();;
        textures.push_back(texture);
    }
    return textures;    
}