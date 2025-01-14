#pragma once

#include <GL/glew.h>
#include <vector>
#include <array>

class VertexArrayObject
{
public:
    // Constructor
    // This constructor initializes a new Vertex Array Object (VAO) by generating a unique ID for it using OpenGL.
    VertexArrayObject() { glGenVertexArrays(1, &ArrayID); }

    // Destructor
    // The destructor deletes the VAO by its unique ID to free up resources when the object goes out of scope.
    ~VertexArrayObject() { glDeleteVertexArrays(1, &ArrayID); }

    // Bind the VAO
    // This method binds the VAO, making it the current VAO to which OpenGL commands will apply.
    void Bind() const { glBindVertexArray(ArrayID); }

    // Unbind the VAO
    // This method unbinds the current VAO by binding VAO 0, which effectively stops using any VAO.
    void Unbind() const { glBindVertexArray(0); }

    // Attach a Vertex Buffer Object (VBO) to the VAO
    // This method binds a Vertex Buffer Object (VBO) to the VAO, specifying it as the current buffer for vertex attributes.
    //
    // Parameters:
    // - vboID: The unique ID of the VBO to attach.
    void AttachVertexBuffer(const GLuint vboID) const { glBindBuffer(GL_ARRAY_BUFFER, vboID); }

    // Attach an Element Buffer Object (EBO) to the VAO
    // This method binds an Element Buffer Object (EBO) to the VAO, specifying it as the current buffer for index data.
    //
    // Parameters:
    // - eboID: The unique ID of the EBO to attach.
    void AttachElementBuffer(const GLuint eboID) const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboID); }

    // Enable vertex attribute arrays and set vertex attribute pointers
    // This method sets up and enables a vertex attribute pointer, which defines how vertex attribute data (like position, color, etc.)
    // is stored in the vertex buffer and how it is passed to the vertex shader.
    //
    // Parameters:
    // - index: Specifies the index of the vertex attribute to be enabled. This corresponds to the layout location in the vertex shader.
    // - size: Specifies the number of components per vertex attribute. For example, a position attribute might have 3 components (x, y, z).
    // - type: Specifies the data type of each component in the array. Common types are GL_FLOAT, GL_INT, etc.
    // - normalized: Specifies whether fixed-point data values should be normalized (GL_TRUE) or converted directly as integers (GL_FALSE).
    // - stride: Specifies the byte offset between consecutive vertex attributes. If attributes are tightly packed, this can be set to 0.
    // - pointer: Specifies a pointer to the first component of the first vertex attribute in the buffer. This is often an offset value
    //            within the vertex buffer object (VBO).
    void EnableAttribute(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* pointer) const
    {
        glVertexAttribPointer(index, size, type, normalized, stride, pointer);
        glEnableVertexAttribArray(index);
    }

    // Get the array ID
    // This inline method returns the unique ID of the VAO.
    //
    // Returns:
    // - The unique ID of the VAO as a GLuint.
    inline const GLuint GetArrayID() const { return ArrayID; }

private:
    GLuint ArrayID;  // The unique ID of the Vertex Array Object (VAO)
};

using VAO = VertexArrayObject;