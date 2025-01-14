#include "EBO.h"

ElementBufferObject::ElementBufferObject()
{
    glGenBuffers(1, &BufferID);
}

ElementBufferObject::~ElementBufferObject()
{
    glDeleteBuffers(1, &BufferID);
}

void ElementBufferObject::Bind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, BufferID);
}

void ElementBufferObject::Unbind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
