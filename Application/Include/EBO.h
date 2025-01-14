#pragma once

#include <GL/glew.h>
#include <vector>
#include <array>

class ElementBufferObject
{
public:
    // Constructor
    // This constructor generates a buffer ID for the Element Buffer Object (EBO) using OpenGL.
    ElementBufferObject();

    // Destructor
    // This destructor deletes the EBO by its buffer ID to free up resources when the object goes out of scope.
    ~ElementBufferObject();

    // Bind the EBO
    // This method binds the EBO, making it the current buffer for the target GL_ELEMENT_ARRAY_BUFFER.
    void Bind() const;

    // Unbind the EBO
    // This method unbinds the EBO by binding the buffer to 0 for the target GL_ELEMENT_ARRAY_BUFFER.
    void Unbind() const;

    // Upload data to the EBO from const void*
    // This method uploads data to the EBO for the target GL_ELEMENT_ARRAY_BUFFER using raw pointers.
    //
    // Parameters:
    // - size: The size in bytes of the data to be uploaded.
    // - data: A pointer to the data to be uploaded.
    // - usage: The expected usage pattern of the data store. Common values are GL_STATIC_DRAW, GL_DYNAMIC_DRAW, and GL_STREAM_DRAW.
    inline void UploadData(GLsizeiptr size, const void* data, GLenum usage = GL_STATIC_DRAW) const
    {
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, usage);
    }

    // Upload data to the EBO from std::vector
    // This method uploads data to the EBO using a std::vector.
    //
    // Parameters:
    // - data: The std::vector containing the data to be uploaded.
    // - usage: The expected usage pattern of the data store.
    template <typename T>
    inline void UploadData(const std::vector<T>& data, GLenum usage = GL_STATIC_DRAW) const
    {
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.size() * sizeof(T), data.data(), usage);
    }

    // Upload data to the EBO from std::array
    // This method uploads data to the EBO using a std::array.
    //
    // Parameters:
    // - data: The std::array containing the data to be uploaded.
    // - usage: The expected usage pattern of the data store.
    template <typename T, std::size_t N>
    inline void UploadData(const std::array<T, N>& data, GLenum usage = GL_STATIC_DRAW) const
    {
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.size() * sizeof(T), data.data(), usage);
    }

    // Get the buffer ID
    // This inline method returns the OpenGL ID of the EBO.
    //
    // Returns:
    // - The unique ID of the buffer as a GLuint.
    inline const GLuint GetBufferID() const { return BufferID; }

private:
    GLuint BufferID;  // OpenGL ID for the Element Buffer Object (EBO)
};

using IndexBufferObject = ElementBufferObject;
using EBO = ElementBufferObject;
using IBO = ElementBufferObject;