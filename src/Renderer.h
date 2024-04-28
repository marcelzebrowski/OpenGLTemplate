#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "VertexArray.h"
#include "Shader.h"
#include "IndexBuffer.h"


#define ASSERT(x) if (!(x)) __debugbreak();
#define GlCall(x) {\
    GlClearError();\
    x;\
    ASSERT(GlLogCall(#x, __FILE__, __LINE__))\
}


void GlClearError();
GLboolean GlLogCall(const char* function, const char* file, int line);

class Renderer
{
private:
    /* data */
public:
    Renderer(/* args */);
    ~Renderer();

    void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;

    void Clear() const;
};

