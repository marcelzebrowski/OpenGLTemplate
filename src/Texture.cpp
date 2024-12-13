#include <iostream>
#include <GL/glew.h>
#include "Texture.hpp"
#include "stb_image.hpp"

Texture::Texture(const char* filePath){
    ID = createTexture(filePath);
}

Texture::~Texture(){
    glDeleteTextures(1, &ID);
    ID = 0;
}

void Texture::attach(){
    glBindTexture(GL_TEXTURE_2D, ID);
}

void Texture::detach(){
    glBindTexture(GL_TEXTURE_2D,0);
}

unsigned int Texture::createTexture(const char* filePath){

    int width, height, nrChannels;

    
    unsigned char* data = stbi_load(filePath,&width,&height,&nrChannels,0);

    if(!data){
        std::cout << "Failed to load texture from file" << std::endl;
        return 0;
    }

    glGenTextures(1,&ID);
    glBindTexture(GL_TEXTURE_2D, ID);

    // set the texture wrapping/filtering options

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // check channels
    GLenum format = (nrChannels == 4) ? GL_RGBA: GL_RGB;
    // load texture to opengl
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

    GLenum error = glGetError();
    if(error != GL_NO_ERROR){
        std::cerr << "OpenGL Error after glTexImage2D:" << error << std::endl;
        stbi_image_free(data);
        return 0;
    }

    if(GLEW_ARB_framebuffer_object){
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    // free memory
    stbi_image_free(data);
    return ID;
}

