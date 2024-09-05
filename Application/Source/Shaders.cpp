#include "Shaders.h"
#include "GL/glew.h"
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>

ShaderSourceProgram ParseShader(const std::string& filepath)
{
    enum class ShaderType
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
            ss[static_cast<int>(currentType)] << line << '\n';
        }
    }

    return {ss[0].str(), ss[1].str()};
}


uint32 CreateShader(const std::string& VertexShader, const std::string& PixelShader)
{
	uint32 Program = glCreateProgram();
	uint32 VS = CompileShader(VertexShader, GL_VERTEX_SHADER);
	uint32 PS = CompileShader(PixelShader, GL_FRAGMENT_SHADER);

	glAttachShader(Program, VS);
	glAttachShader(Program, PS);
	glLinkProgram(Program);
	glValidateProgram(Program);

	glDeleteShader(VS);
	glDeleteShader(PS);

	return Program;
}

uint32 CompileShader(const std::string& Source, uint32 Type)
{
	uint32 id = glCreateShader(Type);
	const char* source = Source.c_str();
	glShaderSource(id, 1, &source, nullptr);
	glCompileShader(id);

	int32 CompileResult;
	glGetShaderiv(id, GL_COMPILE_STATUS, &CompileResult);

	if (CompileResult == GL_FALSE)
	{
		int32 length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)_malloca(length * sizeof(char));
		glGetShaderInfoLog(id, length, &length, message);
		const char* shader_type = Type == GL_VERTEX_SHADER ? "vertex" : "pixel";
		std::cerr << "Failed to compile " << shader_type << " shader!" << std::endl;
		std::cerr << message << std::endl;
		glDeleteShader(id);
		return 0;
	}

	return id;
}
