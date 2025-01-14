#include "Shaders.h"
#include "GL/glew.h"
#include <sstream>
#include <iostream>
#include <fstream>
#include <Platform.h>

Shader::Shader(const std::string& filepath) 
{
    ShaderSourceProgram Source = ParseShader(filepath);
    ShaderID = CreateShader(Source.VertexShader, Source.PixelShader);
}

Shader::~Shader() 
{
    if (!ShaderID) return;
    glDeleteProgram(ShaderID);
}

void Shader::Bind() const
{
    glUseProgram(ShaderID);
}

ShaderSourceProgram Shader::ParseShader(const std::string& filepath)
{
    enum class ShaderType : int8
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
            ss[static_cast<int8>(currentType)] << line << '\n';
        }
    }

    return {ss[0].str(), ss[1].str()};
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

GLuint Shader::CompileShader(const std::string& Source, GLuint Type)
{
    GLuint id = glCreateShader(Type);
    const char* source = Source.c_str();
    glShaderSource(id, 1, &source, nullptr);
    glCompileShader(id);


    GLint CompileResult;
    glGetShaderiv(id, GL_COMPILE_STATUS, &CompileResult);

    if (CompileResult == GL_FALSE)
    {
        GLint length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)_malloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);

        const char* shaderType = Type == GL_VERTEX_SHADER ? "vertex" : "pixel";
        std::cerr << "Failed to compile " << shaderType << " shader!" << std::endl;
        std::cerr << message << std::endl;

        glDeleteShader(id);
        return 0;
    }

    return id;
}