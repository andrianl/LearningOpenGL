#pragma once

#include <GL/glew.h>
#include <vector>
#include <array>

class VertexBufferObject
{
public:
    // Constructor
    // This constructor generates a buffer ID for the Vertex Buffer Object (VBO) using OpenGL.
    VertexBufferObject() { glGenBuffers(1, &BufferID); }

    // Destructor
    // This destructor deletes the VBO by its buffer ID to free up resources when the object goes out of scope.
    ~VertexBufferObject() { glDeleteBuffers(1, &BufferID); }

    // Bind the VBO
    // This method binds the VBO, making it the current buffer for the specified target (default is GL_ARRAY_BUFFER).
    //
    // Parameters:
    // - target: The target to which the buffer object is bound. Common targets are GL_ARRAY_BUFFER for vertex attributes
    //           and GL_ELEMENT_ARRAY_BUFFER for element indices.
    inline void Bind(GLenum target = GL_ARRAY_BUFFER) const { glBindBuffer(target, BufferID); }

    // Unbind the VBO
    // This method unbinds the VBO by binding the buffer to 0 for the specified target (default is GL_ARRAY_BUFFER).
    //
    // Parameters:
    // - target: The target to which the buffer object was bound. Unbinding resets the target.
    inline void Unbind(GLenum target = GL_ARRAY_BUFFER) const { glBindBuffer(target, 0); }

    // Upload data to the VBO
    // This method uploads data to the VBO for the specified target using raw pointers.
    //
    // Parameters:
    // - target: The target to which the buffer object is bound. Common targets are GL_ARRAY_BUFFER and GL_ELEMENT_ARRAY_BUFFER.
    // - size: The size in bytes of the data to be uploaded.
    // - data: A pointer to the data to be uploaded.
    // - usage: The expected usage pattern of the data store. Common values are GL_STATIC_DRAW, GL_DYNAMIC_DRAW, and GL_STREAM_DRAW.
    inline void UploadData(GLenum target, GLsizeiptr size, const void* data, GLenum usage = GL_STATIC_DRAW) const
    {
        glBufferData(target, size, data, usage);
    }

    // Upload data from std::vector
    // This method uploads data to the VBO using a std::vector.
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
    // This method uploads data to the VBO using a std::array.
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
    // This inline method returns the OpenGL ID of the VBO.
    //
    // Returns:
    // - The unique ID of the buffer as a GLuint.
    inline const GLuint GetBufferID() const { return BufferID; }

private:
    GLuint BufferID;  // OpenGL ID for the Vertex Buffer Object (VBO)
};

using VBO = VertexBufferObject;
