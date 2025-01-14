#include "VAO.h"

VertexArrayObject::VertexArrayObject()
{
    glGenVertexArrays(1, &ArrayID);
}

VertexArrayObject::~VertexArrayObject()
{
    glDeleteVertexArrays(1, &ArrayID);
}

void VertexArrayObject::Bind() const 
 {
    glBindVertexArray(ArrayID);
 }

 void VertexArrayObject::Unbind() const
 {
     glBindVertexArray(0);
 }

 void VertexArrayObject::AttachVertexBuffer(const GLuint vboID) const
 {
     glBindBuffer(GL_ARRAY_BUFFER, vboID);
 }

 void VertexArrayObject::AttachElementBuffer(const GLuint eboID) const
 {
     glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboID);
 }

 void VertexArrayObject::EnableAttribute(
     GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* pointer) const
 {
     glVertexAttribPointer(index, size, type, normalized, stride, pointer);
     glEnableVertexAttribArray(index);
 }
