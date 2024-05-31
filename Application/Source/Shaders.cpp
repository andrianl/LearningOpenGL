#include "Shaders.h"
#include "GL/glew.h"
#include <iostream>

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
