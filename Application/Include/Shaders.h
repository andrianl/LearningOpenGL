#pragma once

#include <GL/glew.h>
#include <string>

// Struct to hold the vertex and pixel shader source code
struct ShaderSourceProgram
{
    std::string VertexShader;  // Vertex shader source code
    std::string PixelShader;   // Pixel (fragment) shader source code
};

class Shader
{
public:
    // Default constructor
    // This default constructor does nothing; it is useful for creating an uninitialized Shader object.
    Shader() = default;

    // Constructor that initializes the shader program from a file
    // This constructor reads the shader source code from the specified file, compiles the shaders, and links them into a shader program.
    //
    // Parameters:
    // - filepath: The path to the file containing the shader source code for both the vertex and pixel shaders.
    explicit Shader(const std::string& filepath);

    // Destructor to clean up resources
    // This destructor deletes the shader program to free up GPU resources when the Shader object goes out of scope.
    ~Shader();

    // Copy constructor (default)
    // The default copy constructor is used to create a copy of a Shader object.
    Shader(const Shader&) = default;

    // Copy assignment operator (default)
    // The default copy assignment operator is used to assign the contents of one Shader object to another.
    Shader& operator=(const Shader&) = default;

    // Activates the shader program for use in OpenGL rendering
    // This method binds the shader program, making it the current shader program used for rendering.
    void Bind() const;

    // Returns the shader program ID
    // This method returns the OpenGL ID of the shader program.
    //
    // Returns:
    // - The unique ID of the shader program as a GLuint.
    inline const GLuint GetShaderID() const { return ShaderID; }

private:
    // Parses the shader source file and extracts vertex and pixel shader code
    // This method reads the shader source code from the specified file, separates the vertex and pixel shader code, and returns them as a
    // ShaderSourceProgram struct.
    //
    // Parameters:
    // - filepath: The path to the file containing the shader source code.
    //
    // Returns:
    // - A ShaderSourceProgram struct containing the vertex and pixel shader source code.
    struct ShaderSourceProgram ParseShader(const std::string& filepath);

    // Creates and links a shader program using vertex and pixel shader source code
    // This method creates and compiles the vertex and pixel shaders, attaches them to a shader program, and links the program.
    //
    // Parameters:
    // - VertexShader: The source code for the vertex shader.
    // - PixelShader: The source code for the pixel (fragment) shader.
    //
    // Returns:
    // - The unique ID of the created and linked shader program as a GLuint.
    GLuint CreateShader(const std::string& VertexShader, const std::string& PixelShader);

    // Compiles the given shader source code (vertex or pixel) and returns its ID
    // This method compiles the provided shader source code and returns the shader's OpenGL ID.
    //
    // Parameters:
    // - Source: The source code of the shader to compile.
    // - Type: The type of the shader (e.g., GL_VERTEX_SHADER or GL_FRAGMENT_SHADER).
    //
    // Returns:
    // - The unique ID of the compiled shader as a GLuint.
    GLuint CompileShader(const std::string& Source, GLuint Type);

    GLuint ShaderID = 0;  // OpenGL ID for the shader program
};
