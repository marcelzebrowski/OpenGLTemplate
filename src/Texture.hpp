#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>

class Texture{

private:
    unsigned int ID;

public:
    Texture(const char* filePath);
    ~Texture();
    void attach();
    void detach();

private:
    unsigned int createTexture(const char* filePath);
};



#endif