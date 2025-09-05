#include "Shaders.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <array>

// ------------------------------------------------------------------------
// Shader Constructor and Destructor
// ------------------------------------------------------------------------

Shader::Shader(std::string_view filepath)
{
    // Parse the shader file to retrieve vertex and pixel shader source code
    ShaderSourceProgram source = ParseShader(filepath.data());
    // Create the shader program using the parsed source code
    ShaderID = CreateShader(source);
}

Shader::~Shader()
{
    // Delete the shader program when the Shader object is destroyed
    glDeleteProgram(ShaderID);
}

// ------------------------------------------------------------------------
// Shader Utility Functions
// ------------------------------------------------------------------------

static constexpr size_t ShaderTypeToIndex(ShaderType type)
{
	switch (type)
	{
	case ShaderType::Compute:  return 0;
	case ShaderType::Vertex:   return 1;
	case ShaderType::Hull:     return 2;
	case ShaderType::Domain:   return 3;
	case ShaderType::Geometry: return 4;
	case ShaderType::Pixel:    return 5;
	default:                   return static_cast<size_t>(-1); 
	}
}

ShaderSourceProgram Shader::ParseShader(const std::string& filepath)
{
	std::ifstream stream(filepath);
	if (!stream.is_open())
	{
		throw std::runtime_error("Could not open file: " + filepath);
	}

	std::string line;
	std::stringstream ss[6]; // 0: Compute, 1: Vertex, 2: Hull, 3: Domain, 4: Geometry, 5: Pixel
	ShaderType currentType = ShaderType::None;

	while (std::getline(stream, line))
	{
		if (line.find("#shader") != std::string::npos)
		{
			if (line.find("compute") != std::string::npos)
				currentType = ShaderType::Compute;
			else if (line.find("vertex") != std::string::npos)
				currentType = ShaderType::Vertex;
			else if (line.find("hull") != std::string::npos)
				currentType = ShaderType::Hull;
			else if (line.find("domain") != std::string::npos)
				currentType = ShaderType::Domain;
			else if (line.find("geometry") != std::string::npos)
				currentType = ShaderType::Geometry;
			else if (line.find("pixel") != std::string::npos || line.find("fragment") != std::string::npos)
				currentType = ShaderType::Pixel;
			else
				currentType = ShaderType::None;
		}
		else if (currentType != ShaderType::None)
		{
			size_t idx = ShaderTypeToIndex(currentType);
			if (idx < std::size(ss))
			{
				ss[idx] << line << '\n';
			}
		}
	}

	return {
		ss[0].str(), // Compute
		ss[1].str(), // Vertex
		ss[2].str(), // Hull
		ss[3].str(), // Domain
		ss[4].str(), // Geometry
		ss[5].str()  // Pixel
	};
}

GLuint Shader::CompileShader(const std::string& Source, GLuint Type)
{
	if (Source.empty()) return 0;

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
        auto* message = new char[length];
        glGetShaderInfoLog(id, length, &length, message);
        std::cerr << "Failed to compile shader!" << std::endl;
        std::cerr << message << std::endl;
        delete[] message;
        glDeleteShader(id);
        return 0;
    }
    return id;
}

GLuint Shader::CreateShader(const ShaderSourceProgram& Source)
{
	const GLuint Program = glCreateProgram();

	std::array<GLuint, 6> Shaders = { 0, 0, 0, 0, 0, 0 };
	const GLuint CS = CompileShader(Source.ComputeShader, GL_COMPUTE_SHADER);
    Shaders[0] = CS;

	const GLuint VS = CompileShader(Source.VertexShader, GL_VERTEX_SHADER);
    Shaders[1] = VS;

	const GLuint HS = CompileShader(Source.HullShader, GL_TESS_CONTROL_SHADER);
	Shaders[2] = HS;

	const GLuint DS = CompileShader(Source.DomainShader, GL_TESS_EVALUATION_SHADER);
    Shaders[3] = DS;

	const GLuint GS = CompileShader(Source.GeometryShader, GL_GEOMETRY_SHADER);
    Shaders[4] = GS;

	const GLuint PS = CompileShader(Source.PixelShader, GL_FRAGMENT_SHADER);
    Shaders[5] = PS;

    for (const auto& Shader : Shaders)
    {
        if (Shader != 0)
        {
            glAttachShader(Program, Shader);
		}
    }

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

		if (message != nullptr)
		{
			delete[] message;
		}

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
