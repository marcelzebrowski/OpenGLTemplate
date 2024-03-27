#pragma once

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

class VertexArray
{
private:
    unsigned int m_rendererID;
public:
    VertexArray(/* args */);
    ~VertexArray();

    void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& vbl);
    void Bind() const;
    void Unbind() const;

};

