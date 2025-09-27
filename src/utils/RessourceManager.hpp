#ifndef RESSOURCEMANAGER_HPP
#define RESSOURCEMANAGER_HPP

#include <unordered_map>
#include <string>
#include <memory>
#include "render/Shader.hpp"
#include "render/Texture.hpp"


enum class ShaderID {
    Fraktal,
    Picture
};

enum class TexutureID {
    OverflowLogo
};

class RessourceManager {
public:
    static Shader& loadShader(ShaderID id, const std::string& vsPath, const std::string& fsPath){
        shaders[id] = std::make_unique<Shader>(vsPath, fsPath);
        return *shaders[id];
    }


    static Shader& getShader(ShaderID id){
        return *shaders[id];
    }

    static Texture& getTexture(TexutureID id){
        return *textures[id];
    }


private:
    static inline std::unordered_map<ShaderID, std::unique_ptr<Shader>> shaders;
    static inline std::unordered_map<TexutureID, std::unique_ptr<Texture>> textures;

};

#endif