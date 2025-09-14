#include "Shaders.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>
#include <ranges>
#include <algorithm>
#include <format>

// ------------------------------------------------------------------------
// BaseShader Implementation
// ------------------------------------------------------------------------

template<typename Derived>
GLuint BaseShader<Derived>::CompileShader(std::string_view source, GLuint type) noexcept
{
	if (source.empty()) [[unlikely]]
		return 0;

	const GLuint id = glCreateShader(type);
	const char* src = source.data();
	const GLint length = static_cast<GLint>(source.length());

	glShaderSource(id, 1, &src, &length);
	glCompileShader(id);

	GLint result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);

	if (result == GL_FALSE) [[unlikely]]
	{
		GLint logLength;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &logLength);

		std::string message(logLength, '\0');
		glGetShaderInfoLog(id, logLength, &logLength, message.data());

		constexpr std::string_view shaderTypes[] = {
			"UNKNOWN", "VERTEX", "FRAGMENT", "GEOMETRY", "COMPUTE",
			"TESSELLATION_CONTROL", "TESSELLATION_EVALUATION"
		};

		const auto typeIndex = [type]() constexpr -> std::size_t {
			switch (type) {
			case GL_VERTEX_SHADER: return 1;
			case GL_GEOMETRY_SHADER: return 2;
			case GL_COMPUTE_SHADER: return 3;
			case GL_TESS_CONTROL_SHADER: return 4;
			case GL_TESS_EVALUATION_SHADER: return 5;
			case GL_FRAGMENT_SHADER: return 6;
			default: return 0;
			}
			}();

		std::cerr << std::format("ERROR::SHADER_COMPILATION::{}: {}\n",
			shaderTypes[typeIndex], message);

		glDeleteShader(id);
		return 0;
	}

	return id;
}

template<typename Derived>
GLuint BaseShader<Derived>::LinkProgram(std::span<const GLuint> shaders) noexcept
{
	const GLuint program = glCreateProgram();

	// Attach all valid shaders
	for (const GLuint shader : shaders | std::views::filter([](GLuint id) { return id != 0; }))
	{
		glAttachShader(program, shader);
	}

	glLinkProgram(program);

	// Check linking status
	GLint linkStatus;
	glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);

	if (linkStatus == GL_FALSE) [[unlikely]]
	{
		GLint length;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);

		std::string message(length, '\0');
		glGetProgramInfoLog(program, length, &length, message.data());

		std::cerr << std::format("ERROR::SHADER::LINKING_FAILED: {}\n", message);

		// Clean up
		for (const GLuint shader : shaders | std::views::filter([](GLuint id) { return id != 0; }))
		{
			glDeleteShader(shader);
		}

		glDeleteProgram(program);
		return 0;
	}

	// Clean up individual shaders (now linked into program)
	for (const GLuint shader : shaders | std::views::filter([](GLuint id) { return id != 0; }))
	{
		glDeleteShader(shader);
	}

	return program;
}

// Explicit template instantiations
template class BaseShader<GraphicsShader>;
template class BaseShader<ComputeShader>;
template class BaseShader<RayTracingShader>;

// ------------------------------------------------------------------------
// GraphicsShader Implementation
// ------------------------------------------------------------------------

GraphicsShader::GraphicsShader(std::string_view filepath)
{
	const ShaderSources sources = ParseShaderFile(filepath);
	m_shaderID = CreateProgram(sources);
}

GraphicsShader::GraphicsShader(std::string_view vertexSource,
	std::string_view geometrySource,
	std::string_view tessControlSource,
	std::string_view tessEvalSource,
	std::string_view fragmentSource)
{
	const ShaderSources sources{
		.vertex = std::string(vertexSource),
		.geometry = std::string(geometrySource),
		.tessControl = std::string(tessControlSource),
		.tessEval = std::string(tessEvalSource),
		.fragment = std::string(fragmentSource)
	};

	m_shaderID = CreateProgram(sources);
}

GraphicsShader::ShaderSources GraphicsShader::ParseShaderFile(std::string_view filepath)
{
	std::ifstream stream(filepath.data());
	if (!stream.is_open()) [[unlikely]]
	{
		const std::string error = std::format("Could not open graphics shader file: {}", filepath);
		std::cerr << std::format("ERROR::GRAPHICS_SHADER::FILE_NOT_FOUND: {}\n", filepath);
		throw std::runtime_error(error);
	}

	ShaderSources sources{};
	std::array<std::stringstream, 5> streams; // vertex, fragment, geometry, tessControl, tessEval
	int currentType = -1;

	for (std::string line; std::getline(stream, line);)
	{
		if (line.contains("#shader"))
		{
			if (line.contains("vertex"))
				currentType = 0;
			else if (line.contains("geometry"))
				currentType = 1;
			else if (line.contains("hull") || line.contains("tess_control"))
				currentType = 2;
			else if (line.contains("domain") || line.contains("tess_eval"))
				currentType = 3;
			else if (line.contains("fragment") || line.contains("pixel"))
				currentType = 4;
			else if (line.contains("compute")) [[unlikely]]
			{
				const std::string error = "Compute shaders not allowed in graphics shader files";
				std::cerr << "ERROR::GRAPHICS_SHADER::COMPUTE_NOT_ALLOWED\n";
				throw std::runtime_error(error);
			}
			else
				currentType = -1;
		}
		else if (currentType >= 0 && currentType < static_cast<int>(streams.size()))
		{
			streams[currentType] << line << '\n';
		}
	}

	sources.vertex = streams[0].str();
	sources.geometry = streams[1].str();
	sources.tessControl = streams[2].str();
	sources.tessEval = streams[3].str();
	sources.fragment = streams[4].str();

	return sources;
}

GLuint GraphicsShader::CreateProgram(const ShaderSources& sources) noexcept
{
	if (sources.vertex.empty() || sources.fragment.empty()) [[unlikely]]
	{
		std::cerr << "ERROR::GRAPHICS_SHADER::REQUIRED_SHADERS_MISSING\n";
		return 0;
	}

	const std::array shaderIDs{
		CompileShader(sources.vertex, GL_VERTEX_SHADER),
		CompileShader(sources.geometry, GL_GEOMETRY_SHADER),
		CompileShader(sources.tessControl, GL_TESS_CONTROL_SHADER),
		CompileShader(sources.tessEval, GL_TESS_EVALUATION_SHADER),
		CompileShader(sources.fragment, GL_FRAGMENT_SHADER)
	};

	// Check required shaders
	if (shaderIDs[0] == 0 || shaderIDs[4] == 0) [[unlikely]]
	{
		std::cerr << "ERROR::GRAPHICS_SHADER::COMPILATION_FAILED\n";

		for (const GLuint id : shaderIDs | std::views::filter([](GLuint id) { return id != 0; }))
		{
			glDeleteShader(id);
		}
		return 0;
	}

	return LinkProgram(shaderIDs);
}

// ------------------------------------------------------------------------
// ComputeShader Implementation
// ------------------------------------------------------------------------

ComputeShader::ComputeShader(std::string_view filepath)
{
	const std::string computeSource = ParseComputeShader(filepath);
	m_shaderID = CreateProgram(computeSource);
}

//ComputeShader::ComputeShader(std::string_view computeSource)
//{
//	m_shaderID = CreateProgram(computeSource);
//}

glm::uvec3 ComputeShader::GetWorkGroupSize() const noexcept
{
	if (m_shaderID == 0) [[unlikely]]
		return glm::uvec3(0);

	std::array<GLint, 3> workGroupSize{};
	glGetProgramiv(m_shaderID, GL_COMPUTE_WORK_GROUP_SIZE, workGroupSize.data());

	return {workGroupSize[0], workGroupSize[1], workGroupSize[2]};
}

std::string ComputeShader::ParseComputeShader(std::string_view filepath)
{
	std::ifstream stream(filepath.data());
	if (!stream.is_open()) [[unlikely]]
	{
		const std::string error = std::format("Could not open compute shader file: {}", filepath);
		std::cerr << std::format("ERROR::COMPUTE_SHADER::FILE_NOT_FOUND: {}\n", filepath);
		throw std::runtime_error(error);
	}

	std::stringstream computeStream;
	bool inComputeSection = false;

	for (std::string line; std::getline(stream, line);)
	{
		if (line.contains("#shader"))
		{
			if (line.contains("compute"))
			{
				inComputeSection = true;
			}
			else
			{
				if (line.contains("vertex") || line.contains("fragment") ||
					line.contains("geometry") || line.contains("hull") ||
					line.contains("domain"))
				{
					std::cerr << "WARNING::COMPUTE_SHADER::GRAPHICS_SHADER_IGNORED\n";
				}
				inComputeSection = false;
			}
		}
		else if (inComputeSection)
		{
			computeStream << line << '\n';
		}
	}

	std::string result = computeStream.str();
	if (result.empty()) [[unlikely]]
	{
		const std::string error = std::format("No compute shader found in: {}", filepath);
		std::cerr << "ERROR::COMPUTE_SHADER::NO_SOURCE_FOUND\n";
		throw std::runtime_error(error);
	}

	return result;
}

GLuint ComputeShader::CreateProgram(std::string_view computeSource) noexcept
{
	if (computeSource.empty()) [[unlikely]]
	{
		std::cerr << "ERROR::COMPUTE_SHADER::EMPTY_SOURCE\n";
		return 0;
	}

	const GLuint computeShader = CompileShader(computeSource, GL_COMPUTE_SHADER);
	if (computeShader == 0) [[unlikely]]
		return 0;

	const std::array shaderIDs{ computeShader };
	return LinkProgram(shaderIDs);
}

// ------------------------------------------------------------------------
// RayTracingShader Implementation (Placeholder)
// ------------------------------------------------------------------------

RayTracingShader::RayTracingShader(std::string_view filepath)
{
	const RTShaderSources sources = ParseRTShaders(filepath);
	m_shaderID = CreateProgram(sources);
}

void RayTracingShader::TraceRays(GLuint width, GLuint height, GLuint depth) const noexcept
{
	std::cerr << "WARNING::RAY_TRACING::NOT_IMPLEMENTED\n";
}

RayTracingShader::RTShaderSources RayTracingShader::ParseRTShaders(std::string_view filepath)
{
	std::ifstream stream(filepath.data());
	if (!stream.is_open()) [[unlikely]]
	{
		const std::string error = std::format("Could not open RT shader file: {}", filepath);
		std::cerr << std::format("ERROR::RAY_TRACING::FILE_NOT_FOUND: {}\n", filepath);
		throw std::runtime_error(error);
	}

	RTShaderSources sources{};
	std::array<std::stringstream, 6> streams;
	int currentType = -1;

	for (std::string line; std::getline(stream, line);)
	{
		if (line.contains("#shader"))
		{
			if (line.contains("raygen") || line.contains("ray_generation"))
				currentType = 0;
			else if (line.contains("closesthit") || line.contains("closest_hit"))
				currentType = 1;
			else if (line.contains("anyhit") || line.contains("any_hit"))
				currentType = 2;
			else if (line.contains("miss"))
				currentType = 3;
			else if (line.contains("intersection"))
				currentType = 4;
			else if (line.contains("callable"))
				currentType = 5;
			else
				currentType = -1;
		}
		else if (currentType >= 0 && currentType < static_cast<int>(streams.size()))
		{
			streams[currentType] << line << '\n';
		}
	}

	sources.rayGen = streams[0].str();
	sources.closestHit = streams[1].str();
	sources.anyHit = streams[2].str();
	sources.miss = streams[3].str();
	sources.intersection = streams[4].str();
	sources.callable = streams[5].str();

	return sources;
}

GLuint RayTracingShader::CreateProgram(const RTShaderSources& sources) noexcept
{
	std::cerr << "WARNING::RAY_TRACING::NOT_IMPLEMENTED\n";
	return 0;
}

// ------------------------------------------------------------------------
// Legacy Shader Implementation
// ------------------------------------------------------------------------

Shader::Shader(std::string_view filepath)
{
	// Quick scan to determine shader type
	std::ifstream stream(filepath.data());
	if (!stream.is_open()) [[unlikely]]
	{
		const std::string error = std::format("Could not open shader file: {}", filepath);
		std::cerr << std::format("ERROR::LEGACY_SHADER::FILE_NOT_FOUND: {}\n", filepath);
		throw std::runtime_error(error);
	}

	bool hasCompute = false, hasGraphics = false, hasRayTracing = false;

	for (std::string line; std::getline(stream, line);)
	{
		if (line.contains("#shader"))
		{
			if (line.contains("compute"))
				hasCompute = true;
			else if (line.contains("vertex") || line.contains("fragment") ||
				line.contains("geometry") || line.contains("hull") ||
				line.contains("domain"))
				hasGraphics = true;
			else if (line.contains("raygen") || line.contains("closesthit") ||
				line.contains("anyhit") || line.contains("miss"))
				hasRayTracing = true;
		}
	}

	// Create appropriate shader type using placement new
	if (hasCompute && !hasGraphics && !hasRayTracing)
	{
		m_type = Type::Compute;
		std::construct_at(&m_shader.compute, filepath);
	}
	else if (hasRayTracing && !hasCompute && !hasGraphics)
	{
		m_type = Type::RayTracing;
		std::construct_at(&m_shader.rayTracing, filepath);
	}
	else if (hasGraphics && !hasCompute && !hasRayTracing)
	{
		m_type = Type::Graphics;
		std::construct_at(&m_shader.graphics, filepath);
	}
	else [[unlikely]]
	{
		const std::string error = "Cannot mix different shader pipeline types";
		std::cerr << "ERROR::LEGACY_SHADER::MIXED_TYPES\n";
		throw std::runtime_error(error);
	}
}

void Shader::DestroyShader() noexcept
{
	switch (m_type)
	{
	case Type::Graphics:
		std::destroy_at(&m_shader.graphics);
		break;
	case Type::Compute:
		std::destroy_at(&m_shader.compute);
		break;
	case Type::RayTracing:
		std::destroy_at(&m_shader.rayTracing);
		break;
	}
}

void Shader::Bind() const noexcept
{
	switch (m_type)
	{
	case Type::Graphics:
		m_shader.graphics.Bind();
		break;
	case Type::Compute:
		m_shader.compute.Bind();
		break;
	case Type::RayTracing:
		m_shader.rayTracing.Bind();
		break;
	}
}

void Shader::Unbind() noexcept
{
	BaseShader<GraphicsShader>::Unbind(); // All types use the same unbind
}

GLuint Shader::GetShaderID() const noexcept
{
	switch (m_type)
	{
	case Type::Graphics:
		return m_shader.graphics.GetShaderID();
	case Type::Compute:
		return m_shader.compute.GetShaderID();
	case Type::RayTracing:
		return m_shader.rayTracing.GetShaderID();
	}
	return 0;
}

bool Shader::IsValid() const noexcept
{
	switch (m_type)
	{
	case Type::Graphics:
		return m_shader.graphics.IsValid();
	case Type::Compute:
		return m_shader.compute.IsValid();
	case Type::RayTracing:
		return m_shader.rayTracing.IsValid();
	}
	return false;
}

void Shader::SetBool(std::string_view name, bool value) const noexcept
{
	switch (m_type)
	{
	case Type::Graphics:
		m_shader.graphics.SetBool(name, value);
		break;
	case Type::Compute:
		m_shader.compute.SetBool(name, value);
		break;
	case Type::RayTracing:
		m_shader.rayTracing.SetBool(name, value);
		break;
	}
}

void Shader::SetInt(std::string_view name, int value) const noexcept
{
	switch (m_type)
	{
	case Type::Graphics:
		m_shader.graphics.SetInt(name, value);
		break;
	case Type::Compute:
		m_shader.compute.SetInt(name, value);
		break;
	case Type::RayTracing:
		m_shader.rayTracing.SetInt(name, value);
		break;
	}
}

void Shader::SetFloat(std::string_view name, float value) const noexcept
{
	switch (m_type)
	{
	case Type::Graphics:
		m_shader.graphics.SetFloat(name, value);
		break;
	case Type::Compute:
		m_shader.compute.SetFloat(name, value);
		break;
	case Type::RayTracing:
		m_shader.rayTracing.SetFloat(name, value);
		break;
	}
}

void Shader::SetVec2(std::string_view name, const glm::vec2& value) const noexcept
{
	switch (m_type)
	{
	case Type::Graphics:
		m_shader.graphics.SetVec2(name, value);
		break;
	case Type::Compute:
		m_shader.compute.SetVec2(name, value);
		break;
	case Type::RayTracing:
		m_shader.rayTracing.SetVec2(name, value);
		break;
	}
}

void Shader::SetVec3(std::string_view name, const glm::vec3& value) const noexcept
{
	switch (m_type)
	{
	case Type::Graphics:
		m_shader.graphics.SetVec3(name, value);
		break;
	case Type::Compute:
		m_shader.compute.SetVec3(name, value);
		break;
	case Type::RayTracing:
		m_shader.rayTracing.SetVec3(name, value);
		break;
	}
}

void Shader::SetVec4(std::string_view name, const glm::vec4& value) const noexcept
{
	switch (m_type)
	{
	case Type::Graphics:
		m_shader.graphics.SetVec4(name, value);
		break;
	case Type::Compute:
		m_shader.compute.SetVec4(name, value);
		break;
	case Type::RayTracing:
		m_shader.rayTracing.SetVec4(name, value);
		break;
	}
}

void Shader::SetMat4(std::string_view name, const glm::mat4& mat) const noexcept
{
	switch (m_type)
	{
	case Type::Graphics:
		m_shader.graphics.SetMat4(name, mat);
		break;
	case Type::Compute:
		m_shader.compute.SetMat4(name, mat);
		break;
	case Type::RayTracing:
		m_shader.rayTracing.SetMat4(name, mat);
		break;
	}
}

void Shader::Dispatch(GLuint x, GLuint y, GLuint z) const noexcept
{
	if (m_type == Type::Compute) [[likely]]
	{
		m_shader.compute.Dispatch(x, y, z);
	}
	else [[unlikely]]
	{
		std::cerr << "ERROR::LEGACY_SHADER::INVALID_DISPATCH\n";
	}
}

void Shader::TraceRays(GLuint width, GLuint height, GLuint depth) const noexcept
{
	if (m_type == Type::RayTracing) [[likely]]
	{
		m_shader.rayTracing.TraceRays(width, height, depth);
	}
	else [[unlikely]]
	{
		std::cerr << "ERROR::LEGACY_SHADER::INVALID_TRACE_RAYS\n";
	}
}