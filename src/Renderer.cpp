#include "Renderer.h"
#include <iostream>

void GlClearError(){
    while (glGetError() != GL_NO_ERROR)
    {
        /* code */
    }
}

GLboolean GlLogCall(const char* function, const char* file, int line){
    while (GLenum error = glGetError())
    {
        /* code */
        std::cout << "[OpenGL Error] (" << error << ")" 
        << " file: " << file
        << " function " << function << " line " 
        << line << std::endl;
        return GL_FALSE;
    }
    return GL_TRUE;
    
}


Renderer::Renderer(/* args */)
{
}

Renderer::~Renderer()
{
}
