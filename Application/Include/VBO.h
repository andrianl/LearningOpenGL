#pragma once

#include <GL/glew.h>
#include <vector>
#include <array>
#include "Vertex.h"

class VertexBufferObject
{
public:
    // Constructor
    // Generates a buffer ID for the Vertex Buffer Object (VBO) using OpenGL.
    VertexBufferObject();

    // Destructor
    // Deletes the VBO using its buffer ID to free up resources when the object goes out of scope.
    ~VertexBufferObject();

    // Bind the VBO
    // Binds the VBO to the specified target (default is GL_ARRAY_BUFFER), making it the current buffer.
    //
    // Parameters:
    // - target: The target to which the buffer object is bound (e.g., GL_ARRAY_BUFFER for vertex attributes
    //           or GL_ELEMENT_ARRAY_BUFFER for element indices).
    inline void Bind(GLenum target = GL_ARRAY_BUFFER) const { glBindBuffer(target, BufferID); }

    // Unbind the VBO
    // Unbinds the VBO by binding the buffer to 0 for the specified target (default is GL_ARRAY_BUFFER).
    //
    // Parameters:
    // - target: The target from which the buffer object will be unbound.
    inline void Unbind(GLenum target = GL_ARRAY_BUFFER) const { glBindBuffer(target, 0); }

    // Upload data to the VBO
    // Uploads data to the VBO for the specified target using raw pointers.
    //
    // Parameters:
    // - target: The target to which the buffer object is bound.
    // - size: The size in bytes of the data to be uploaded.
    // - data: A pointer to the data to be uploaded.
    // - usage: The expected usage pattern of the data store (e.g., GL_STATIC_DRAW, GL_DYNAMIC_DRAW, or GL_STREAM_DRAW).
    inline void UploadData(GLenum target, GLsizeiptr size, const void* data, GLenum usage = GL_STATIC_DRAW) const
    {
        glBufferData(target, size, data, usage);
    }

    // Upload data from std::vector
    // Uploads data to the VBO using a std::vector.
    //
    // Parameters:
    // - target: The target to which the buffer object is bound.
    // - data: The std::vector containing the data to be uploaded.
    // - usage: The expected usage pattern of the data store.
    template <typename T>
    inline void UploadData(GLenum target, const std::vector<T>& data, GLenum usage = GL_STATIC_DRAW) const
    {
        glBufferData(target, data.size() * sizeof(T), data.data(), usage);
    }

    // Upload data from std::array
    // Uploads data to the VBO using a std::array.
    //
    // Parameters:
    // - target: The target to which the buffer object is bound.
    // - data: The std::array containing the data to be uploaded.
    // - usage: The expected usage pattern of the data store.
    template <typename T, std::size_t N>
    inline void UploadData(GLenum target, const std::array<T, N>& data, GLenum usage = GL_STATIC_DRAW) const
    {
        glBufferData(target, data.size() * sizeof(T), data.data(), usage);
    }

    // Get the buffer ID
    // Returns the unique OpenGL ID of the VBO.
    //
    // Returns:
    // - The OpenGL buffer ID as a GLuint.
    inline const GLuint GetBufferID() const { return BufferID; }

private:
    GLuint BufferID;  // OpenGL ID for the Vertex Buffer Object (VBO)
};

using VBO = VertexBufferObject;
