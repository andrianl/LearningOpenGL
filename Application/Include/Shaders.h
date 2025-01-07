#pragma once

#include <string>
#include <Platform.h>

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
    Shader() = default;

    // Constructor that initializes the shader program from a file
    explicit Shader(const std::string& filepath);

    // Destructor to clean up resources
    ~Shader();

    Shader(const Shader&) = default;
    Shader& operator=(const Shader&) = default;

    // Activates the shader program for use in OpenGL rendering
    void Use() const;

    // Returns the shader program ID
    uint32 GetShaderID() const { return ShaderID; }

private:
    // Parses the shader source file and extracts vertex and pixel shader code
    struct ShaderSourceProgram ParseShader(const std::string& filepath);

    // Creates and links a shader program using vertex and pixel shader source code
    uint32 CreateShader(const std::string& VertexShader, const std::string& PixelShader);

    // Compiles the given shader source code (vertex or pixel) and returns its ID
    uint32 CompileShader(const std::string& Source, uint32 Type);

    uint32 ShaderID = 0;  // OpenGL ID for the shader program
};
