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

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const{
    shader.Bind();
    va.Bind();
    ib.Bind();
    GlCall(glDrawElements(GL_TRIANGLES,ib.GetCount(), GL_UNSIGNED_INT, nullptr));
};


void Renderer::Clear() const{
     GlCall(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));
     GlCall(glClear(GL_COLOR_BUFFER_BIT));
}
