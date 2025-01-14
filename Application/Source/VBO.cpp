#include "VBO.h"

VertexBufferObject::VertexBufferObject()
{
    glGenBuffers(1, &BufferID);
}

VertexBufferObject::~VertexBufferObject()
{
    glDeleteBuffers(1, &BufferID);
}