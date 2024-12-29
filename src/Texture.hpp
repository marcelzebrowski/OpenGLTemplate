#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include "Shader.hpp"

class Texture{

private:
    unsigned int ID;
    int unit;

public:
    Texture(const char* filePath, int unit);
    ~Texture();
    void attach(const Shader& shader, const std::string uniform);
    void attach(const Shader* shader, const std::string uniform);
    void detach();

private:
    unsigned int createTexture(const char* filePath);
   
};



#endif