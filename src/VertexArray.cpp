#include "VertexArray.h"
#include "Renderer.h"

VertexArray::VertexArray(/* args */):m_rendererID(0)
{
    GlCall(glGenVertexArrays(1,&m_rendererID));
}

VertexArray::~VertexArray()
{
    GlCall(glDeleteVertexArrays(1,&m_rendererID));
}


void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& vbl){
    Bind();
    vb.Bind();
    const auto& elements = vbl.GetElements();
    unsigned int offset = 0;

    for(unsigned int i = 0; i < elements.size(); i++){
        auto& element = elements[i];
        GlCall(glEnableVertexAttribArray(i)); 
        GlCall(glVertexAttribPointer(i,element.count,element.type,element.normalized, vbl.GetStride(),(const void*)offset)); // links the buffer with vao
        offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
    }
}

void VertexArray::Bind() const{
    GlCall(glBindVertexArray(m_rendererID));
}

void VertexArray::Unbind() const{
    GlCall(glBindVertexArray(0));
}