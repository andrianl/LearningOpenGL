#include "Shaders.h"
#include <iostream>
#include <fstream>
#include <sstream>

// ------------------------------------------------------------------------
// Shader Constructor and Destructor
// ------------------------------------------------------------------------

Shader::Shader(std::string_view filepath)
{
    // Parse the shader file to retrieve vertex and pixel shader source code
    ShaderSourceProgram source = ParseShader(filepath.data());
    // Create the shader program using the parsed source code
    ShaderID = CreateShader(source.VertexShader, source.PixelShader);
}

Shader::~Shader()
{
    // Delete the shader program when the Shader object is destroyed
    glDeleteProgram(ShaderID);
}

// ------------------------------------------------------------------------
// Shader Utility Functions
// ------------------------------------------------------------------------

ShaderSourceProgram Shader::ParseShader(const std::string& filepath)
{
    enum class ShaderType : int8_t
    {
        None = -1,
        Vertex = 0,
        Pixel = 1
    };

    std::ifstream stream(filepath);
    if (!stream.is_open())
    {
        throw std::runtime_error("Could not open file: " + filepath);
    }

    std::string line;
    std::stringstream ss[2];
    ShaderType currentType = ShaderType::None;

    while (getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
            {
                currentType = ShaderType::Vertex;
            }
            else if (line.find("pixel") != std::string::npos)
            {
                currentType = ShaderType::Pixel;
            }
        }
        else if (currentType != ShaderType::None)
        {
            ss[static_cast<int8_t>(currentType)] << line << '\n';
        }
    }

    return {ss[0].str(), ss[1].str()};
}

GLuint Shader::CompileShader(const std::string& Source, GLuint Type)
{
    GLuint id = glCreateShader(Type);
    const char* src = Source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    // Error handling for shader compilation
    GLint result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        GLint length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = new char[length];
        glGetShaderInfoLog(id, length, &length, message);
        std::cerr << "Failed to compile shader!" << std::endl;
        std::cerr << message << std::endl;
        delete[] message;
        glDeleteShader(id);
        return 0;
    }
    return id;
}

GLuint Shader::CreateShader(const std::string& VertexShader, const std::string& PixelShader)
{
    GLuint Program = glCreateProgram();
    GLuint VS = CompileShader(VertexShader, GL_VERTEX_SHADER);
    GLuint PS = CompileShader(PixelShader, GL_FRAGMENT_SHADER);

    glAttachShader(Program, VS);
    glAttachShader(Program, PS);
    glLinkProgram(Program);

    GLint linkStatus;
    glGetProgramiv(Program, GL_LINK_STATUS, &linkStatus);
    if (linkStatus == GL_FALSE)
    {
        GLint length;
        glGetProgramiv(Program, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)_malloca(length * sizeof(char));
        glGetProgramInfoLog(Program, length, &length, message);
        std::cerr << "Failed to link shader program!" << std::endl;
        std::cerr << message << std::endl;
        delete[] message;
        glDeleteProgram(Program);
        return 0;
    }

    glValidateProgram(Program);
    GLint validateStatus;
    glGetProgramiv(Program, GL_VALIDATE_STATUS, &validateStatus);
    if (validateStatus == GL_FALSE)
    {
        std::cerr << "Shader program validation failed!" << std::endl;
    }

    glDeleteShader(VS);
    glDeleteShader(PS);

    return Program;
}

void Shader::Bind() const
{
    glUseProgram(ShaderID);
}

// ------------------------------------------------------------------------
// Uniform Setter Functions Implementation
// ------------------------------------------------------------------------

GLint Shader::GetUniformLocation(std::string_view name) const
{
    return glGetUniformLocation(ShaderID, name.data());
}

void Shader::SetBool(std::string_view name, bool value) const
{
    glUniform1i(glGetUniformLocation(ShaderID, name.data()), static_cast<int>(value));
}

void Shader::SetInt(std::string_view name, int value) const
{
    glUniform1i(glGetUniformLocation(ShaderID, name.data()), value);
}

void Shader::SetFloat(std::string_view name, float value) const
{
    glUniform1f(glGetUniformLocation(ShaderID, name.data()), value);
}

void Shader::SetVec2(std::string_view name, const glm::vec2& value) const
{
    glUniform2fv(glGetUniformLocation(ShaderID, name.data()), 1, &value[0]);
}

void Shader::SetVec2(std::string_view name, float x, float y) const
{
    glUniform2f(glGetUniformLocation(ShaderID, name.data()), x, y);
}

void Shader::SetVec3(std::string_view name, const glm::vec3& value) const
{
    glUniform3fv(glGetUniformLocation(ShaderID, name.data()), 1, &value[0]);
}

void Shader::SetVec3(std::string_view name, float x, float y, float z) const
{
    glUniform3f(glGetUniformLocation(ShaderID, name.data()), x, y, z);
}

void Shader::SetVec4(std::string_view name, const glm::vec4& value) const
{
    glUniform4fv(glGetUniformLocation(ShaderID, name.data()), 1, &value[0]);
}

void Shader::SetVec4(std::string_view name, float x, float y, float z, float w) const
{
    glUniform4f(glGetUniformLocation(ShaderID, name.data()), x, y, z, w);
}

void Shader::SetMat2(std::string_view name, const glm::mat2& mat) const
{
    glUniformMatrix2fv(glGetUniformLocation(ShaderID, name.data()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::SetMat3(std::string_view name, const glm::mat3& mat) const
{
    glUniformMatrix3fv(glGetUniformLocation(ShaderID, name.data()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::SetMat4(std::string_view name, const glm::mat4& mat) const
{
    glUniformMatrix4fv(glGetUniformLocation(ShaderID, name.data()), 1, GL_FALSE, &mat[0][0]);
}
