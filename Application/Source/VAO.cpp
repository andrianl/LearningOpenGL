#include "VAO.h"

// Constructor
VertexArrayObject::VertexArrayObject()
{
    glGenVertexArrays(1, &ArrayID);
}

// Destructor
VertexArrayObject::~VertexArrayObject()
{
    glDeleteVertexArrays(1, &ArrayID);
}

// Bind the VAO
void VertexArrayObject::Bind() const
{
    glBindVertexArray(ArrayID);
}

// Unbind the VAO
void VertexArrayObject::Unbind() const
{
    glBindVertexArray(0);
}

// Attach a Vertex Buffer Object (VBO) to the VAO
void VertexArrayObject::AttachVertexBuffer(const GLuint vboID) const
{
    glBindBuffer(GL_ARRAY_BUFFER, vboID);
}

// Attach an Element Buffer Object (EBO) to the VAO
void VertexArrayObject::AttachElementBuffer(const GLuint eboID) const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboID);
}

// Enable vertex attribute arrays and set vertex attribute pointers
void VertexArrayObject::EnableAttribute(
    GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* pointer) const
{
    glVertexAttribPointer(index, size, type, normalized, stride, pointer);
    glEnableVertexAttribArray(index);
}

// GlobalVAO implementation