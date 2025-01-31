#pragma once

#include <GL/glew.h>
#include <vector>
#include <array>
#include "Vertex.h"

class VertexArrayObject
{
public:
    // Constructor
    // Initializes a new Vertex Array Object (VAO) by generating a unique ID using OpenGL.
    VertexArrayObject();

    // Destructor
    // Deletes the VAO using its unique ID to free resources when the object goes out of scope.
    ~VertexArrayObject();

    // Bind the VAO
    // Binds the VAO, making it the current VAO for subsequent OpenGL commands.
    void Bind() const;

    // Unbind the VAO
    // Unbinds the current VAO by binding VAO 0, effectively stopping the use of any VAO.
    void Unbind() const;

    // Attach a Vertex Buffer Object (VBO) to the VAO
    // Binds a Vertex Buffer Object (VBO) to the VAO, specifying it as the current buffer for vertex attributes.
    //
    // Parameters:
    // - vboID: The unique ID of the VBO to attach.
    void AttachVertexBuffer(const GLuint vboID) const;

    // Attach an Element Buffer Object (EBO) to the VAO
    // Binds an Element Buffer Object (EBO) to the VAO, specifying it as the current buffer for index data.
    //
    // Parameters:
    // - eboID: The unique ID of the EBO to attach.
    void AttachElementBuffer(const GLuint eboID) const;

    // Enable vertex attribute arrays and set vertex attribute pointers
    // Sets up and enables a vertex attribute pointer, which defines how vertex attribute data (e.g., position, color)
    // is stored in the vertex buffer and passed to the vertex shader.
    //
    // Parameters:
    // - index: The index of the vertex attribute to enable (corresponds to the layout location in the shader).
    // - size: The number of components per vertex attribute (e.g., 3 for a vec3 position).
    // - type: The data type of each component (e.g., GL_FLOAT, GL_INT, etc.).
    // - normalized: Specifies whether fixed-point data values should be normalized (GL_TRUE) or converted directly as integers (GL_FALSE).
    // - stride: The byte offset between consecutive vertex attributes. If attributes are tightly packed, this can be set to 0.
    // - pointer: A pointer to the first component of the first vertex attribute in the buffer (often an offset).
    void EnableAttribute(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* pointer) const;

    // Template function to enable vertex attributes for a given VertexType.
    // Uses the VertexLayout specialization for the provided VertexType.
    template <typename VertexType>
    void EnableVertexAttributes() const
    {
        auto attributes = VertexLayout<VertexType>::GetAttributes();
        for (const auto& attrib : attributes)
        {
            // The pointer is specified as an offset (cast to const void*) assuming the VBO is already bound.
            EnableAttribute(attrib.index, attrib.size, attrib.type, attrib.normalized, attrib.stride, (const void*)(attrib.offset));
        }
    }

    // Get the array ID
    // Returns the unique OpenGL ID of the VAO.
    //
    // Returns:
    // - The VAO's unique ID as a GLuint.
    inline const GLuint GetArrayID() const { return ArrayID; }

private:
    GLuint ArrayID;  // Unique OpenGL ID for the Vertex Array Object (VAO)
};

using VAO = VertexArrayObject;

class GlobalVAO
{
public:
    // Get the instance of GlobalVAO (Singleton)
    // Returns a reference to the single instance of GlobalVAO.
    static GlobalVAO& GetInstance()
    {
        static GlobalVAO instance;  // Guaranteed to be instantiated on first use and destroyed properly.
        return instance;
    }

    // Initialize the Global VAO
    // Generates a unique ID for the Global VAO using OpenGL.
    void Initialize() { glGenVertexArrays(1, &ArrayID); }

    // Delete the Global VAO
    // Deletes the Global VAO using its unique ID to free resources.
    void Delete() const { glDeleteVertexArrays(1, &ArrayID); }

    // Bind the Global VAO
    // Binds the Global VAO, making it the current VAO for subsequent OpenGL commands.
    void Bind() const { glBindVertexArray(ArrayID); }

    // Unbind the Global VAO
    // Unbinds the current VAO by binding VAO 0, effectively stopping the use of any VAO.
    void Unbind() const { glBindVertexArray(0); }

    // Attach a Vertex Buffer Object (VBO) to the Global VAO
    // Binds a Vertex Buffer Object (VBO) to the Global VAO for vertex attributes.
    //
    // Parameters:
    // - vboID: The unique ID of the VBO to attach.
    void AttachVertexBuffer(const GLuint vboID) const { glBindBuffer(GL_ARRAY_BUFFER, vboID); }

    // Attach an Element Buffer Object (EBO) to the Global VAO
    // Binds an Element Buffer Object (EBO) to the Global VAO for index data.
    //
    // Parameters:
    // - eboID: The unique ID of the EBO to attach.
    void AttachElementBuffer(const GLuint eboID) const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboID); }

    // Enable vertex attribute arrays and set vertex attribute pointers for the Global VAO
    // Sets up and enables a vertex attribute pointer, defining how vertex attribute data is stored and passed to the shader.
    //
    // Parameters:
    // - index: The index of the vertex attribute to enable (corresponds to the layout location in the shader).
    // - size: The number of components per vertex attribute (e.g., 3 for a vec3 position).
    // - type: The data type of each component (e.g., GL_FLOAT, GL_INT, etc.).
    // - normalized: Specifies whether fixed-point data values should be normalized (GL_TRUE) or converted directly as integers (GL_FALSE).
    // - stride: The byte offset between consecutive vertex attributes. If attributes are tightly packed, this can be set to 0.
    // - pointer: A pointer to the first component of the first vertex attribute in the buffer (often an offset).
    void EnableAttribute(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* pointer) const
    {
        glVertexAttribPointer(index, size, type, normalized, stride, pointer);
        glEnableVertexAttribArray(index);
    }

    // Template function to enable vertex attributes for a given VertexType.
    // Uses the VertexLayout specialization for the provided VertexType.
    template <typename VertexType>
    void EnableVertexAttributes() const
    {
        auto attributes = VertexLayout<VertexType>::GetAttributes();
        for (const auto& attrib : attributes)
        {
            // The pointer is specified as an offset (cast to const void*) assuming the VBO is already bound.
            EnableAttribute(attrib.index, attrib.size, attrib.type, attrib.normalized, attrib.stride, (const void*)(attrib.offset));
        }
    }

    // Get the array ID
    // Returns the unique OpenGL ID of the Global VAO.
    //
    // Returns:
    // - The Global VAO's unique ID as a GLuint.
    GLuint GetArrayID() const { return ArrayID; }

private:
    GLuint ArrayID;  // Unique OpenGL ID for the Global VAO

    // Private constructor to prevent instantiation
    GlobalVAO() : ArrayID(0) {}

    // Delete copy constructor and assignment operator to prevent copying
    GlobalVAO(const GlobalVAO&) = delete;
    GlobalVAO& operator=(const GlobalVAO&) = delete;
};
