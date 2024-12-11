#include <iostream>
#include "Texture.hpp"
#include "stb_image.hpp"

Texture::Texture(const char* filePath){
    ID = createTexture(filePath);
}

Texture::~Texture(){

}

void Texture::attach(){

}

void Texture::detach(){
    
}

unsigned int Texture::createTexture(const char* filePath){

    int width, height, nrChannels;

    unsigned char* data = stbi_load(filePath,&width,&height,&nrChannels,0);

    if(data){
        std::cout << "successfull" << std::endl;
    }else{
        std::cout << "Failed to load texture from file" << std::endl;
    }

    return -1;
}