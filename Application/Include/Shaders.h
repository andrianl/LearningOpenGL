#pragma once

#include <GL/glew.h>
#include <string>
#include <string_view>
#include <array>
#include <concepts>
#include <span>
#include "glm/glm.hpp"

#if __has_include(<flat_map>)
#include <flat_map>
template<typename K, typename V>
using fast_map = std::flat_map<K, V>;
#else
#include <unordered_map>
template<typename K, typename V>
using fast_map = std::unordered_map<K, V>;
#endif

/**
 * @brief Base functionality for all shader types.
 *
 * Non-virtual base class providing common shader operations.
 * Uses CRTP (Curiously Recurring Template Pattern) for zero-cost abstractions.
 */
template<typename Derived>
class BaseShader
{
public:
    /**
     * @brief Default constructor.
     */
    constexpr BaseShader() = default;

    /**
     * @brief Destructor - non-virtual for smaller object size.
     */
    ~BaseShader() noexcept
    {
        if (m_shaderID != 0) [[likely]]
        {
            glDeleteProgram(m_shaderID);
        }
    }

    // Disable copy constructor and assignment
    BaseShader(const BaseShader&) = delete;
    BaseShader& operator=(const BaseShader&) = delete;

    // Enable move semantics
    constexpr BaseShader(BaseShader&& other) noexcept
        : m_shaderID(std::exchange(other.m_shaderID, 0))
        , m_uniformCache(std::move(other.m_uniformCache))
    {
    }

    constexpr BaseShader& operator=(BaseShader&& other) noexcept
    {
        if (this != &other) [[likely]]
        {
            if (m_shaderID != 0)
            {
                glDeleteProgram(m_shaderID);
            }

            m_shaderID = std::exchange(other.m_shaderID, 0);
            m_uniformCache = std::move(other.m_uniformCache);
        }
        return *this;
    }

    /**
     * @brief Activates this shader program for use.
     */
    void Bind() const noexcept
    {
        if (m_shaderID != 0) [[likely]]
        {
            glUseProgram(m_shaderID);
        }
    }

    /**
     * @brief Deactivates the current shader program.
     */
    static void Unbind() noexcept
    {
        glUseProgram(0);
    }

    /**
     * @brief Returns the OpenGL program ID.
     */
    [[nodiscard]] constexpr GLuint GetShaderID() const noexcept { return m_shaderID; }

    /**
     * @brief Checks if the shader is valid.
     */
    [[nodiscard]] constexpr bool IsValid() const noexcept { return m_shaderID != 0; }

    // ------------------------------------------------------------------------
    // Uniform Setter Functions with optional caching
    // ------------------------------------------------------------------------

    /**
     * @brief Gets uniform location with caching.
     */
	[[nodiscard]] GLint GetUniformLocation(std::string_view& name) const {
		if constexpr (Derived::ENABLE_UNIFORM_CACHING) {
			if (auto it = m_uniformCache.find(name); it != m_uniformCache.end()) [[likely]]
				return it->second;

			GLint location = glGetUniformLocation(m_shaderID, name.data());
			m_uniformCache.emplace(std::string(name), location);
			return location;
		}
		else {
			return glGetUniformLocation(m_shaderID, name.data());
		}
	}

    void SetBool(std::string_view name, bool value) const noexcept
    {
        glUniform1i(GetUniformLocation(name), static_cast<int>(value));
    }

    void SetInt(std::string_view name, int value) const noexcept
    {
        glUniform1i(GetUniformLocation(name), value);
    }

    void SetFloat(std::string_view name, float value) const noexcept
    {
        glUniform1f(GetUniformLocation(name), value);
    }

    void SetVec2(std::string_view name, const glm::vec2& value) const noexcept
    {
        glUniform2fv(GetUniformLocation(name), 1, &value[0]);
    }

    void SetVec2(std::string_view name, float x, float y) const noexcept
    {
        glUniform2f(GetUniformLocation(name), x, y);
    }

    void SetVec3(std::string_view name, const glm::vec3& value) const noexcept
    {
        glUniform3fv(GetUniformLocation(name), 1, &value[0]);
    }

    void SetVec3(std::string_view name, float x, float y, float z) const noexcept
    {
        glUniform3f(GetUniformLocation(name), x, y, z);
    }

    void SetVec4(std::string_view name, const glm::vec4& value) const noexcept
    {
        glUniform4fv(GetUniformLocation(name), 1, &value[0]);
    }

    void SetVec4(std::string_view name, float x, float y, float z, float w) const noexcept
    {
        glUniform4f(GetUniformLocation(name), x, y, z, w);
    }

    void SetMat2(std::string_view name, const glm::mat2& mat) const noexcept
    {
        glUniformMatrix2fv(GetUniformLocation(name), 1, GL_FALSE, &mat[0][0]);
    }

    void SetMat3(std::string_view name, const glm::mat3& mat) const noexcept
    {
        glUniformMatrix3fv(GetUniformLocation(name), 1, GL_FALSE, &mat[0][0]);
    }

    void SetMat4(std::string_view name, const glm::mat4& mat) const noexcept
    {
        glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &mat[0][0]);
    }

protected:
    /**
     * @brief Compiles a shader stage from source.
     */
    [[nodiscard]] static GLuint CompileShader(std::string_view source, GLuint type) noexcept;

    /**
     * @brief Links compiled shaders into a program.
     */
    [[nodiscard]] static GLuint LinkProgram(std::span<const GLuint> shaders) noexcept;

    GLuint m_shaderID = 0;

private:

#if __has_include(<flat_map>)
#else
	struct StringHash {
		using is_transparent = void;
		size_t operator()(std::string_view sv) const noexcept {
			return std::hash<std::string_view>{}(sv);
		}
	};

	struct StringEqual {
		using is_transparent = void;
		bool operator()(std::string_view a, std::string_view b) const noexcept {
			return a == b;
		}
	};
#endif

    mutable fast_map<std::string_view, GLint> m_uniformCache;
};

// ------------------------------------------------------------------------
// Graphics Shader Class
// ------------------------------------------------------------------------

/**
 * @brief Graphics pipeline shader program.
 */
class GraphicsShader : public BaseShader<GraphicsShader>
{
public:
    static constexpr bool ENABLE_UNIFORM_CACHING = true;

    constexpr GraphicsShader() = default;

    /**
     * @brief Constructs from a file.
     */
    explicit GraphicsShader(std::string_view filepath);

    /**
     * @brief Constructs from shader sources.
     */
    GraphicsShader(std::string_view vertexSource,
        std::string_view fragmentSource,
        std::string_view geometrySource = {},
        std::string_view tessControlSource = {},
        std::string_view tessEvalSource = {});

private:
    struct ShaderSources
    {
        std::string vertex;
        std::string geometry;
        std::string tessControl;
        std::string tessEval;
        std::string fragment;
    };

    [[nodiscard]] static ShaderSources ParseShaderFile(std::string_view filepath);
    [[nodiscard]] GLuint CreateProgram(const ShaderSources& sources) noexcept;
};

// ------------------------------------------------------------------------
// Compute Shader Class
// ------------------------------------------------------------------------

/**
 * @brief Compute shader program.
 */
class ComputeShader : public BaseShader<ComputeShader>
{
public:
    static constexpr bool ENABLE_UNIFORM_CACHING = false; // Compute shaders typically set uniforms less frequently

    constexpr ComputeShader() = default;

    /**
     * @brief Constructs from a file.
     */
    explicit ComputeShader(std::string_view filepath);

    /**
     * @brief Constructs from source.
     */
    //explicit ComputeShader(std::string_view computeSource);

    /**
     * @brief Dispatches compute work.
     */
    void Dispatch(GLuint numGroupsX, GLuint numGroupsY = 1, GLuint numGroupsZ = 1) const noexcept
    {
        if (m_shaderID != 0) [[likely]]
        {
            Bind();
            glDispatchCompute(numGroupsX, numGroupsY, numGroupsZ);
        }
    }

    /**
     * @brief Dispatches with memory barrier.
     */
    void DispatchWithBarrier(GLuint numGroupsX, GLuint numGroupsY = 1, GLuint numGroupsZ = 1,
        GLbitfield barriers = GL_SHADER_STORAGE_BARRIER_BIT) const noexcept
    {
        Dispatch(numGroupsX, numGroupsY, numGroupsZ);
        glMemoryBarrier(barriers);
    }

    /**
     * @brief Gets work group size.
     */
    [[nodiscard]] glm::uvec3 GetWorkGroupSize() const noexcept;

private:
    [[nodiscard]] static std::string ParseComputeShader(std::string_view filepath);
    [[nodiscard]] GLuint CreateProgram(std::string_view computeSource) noexcept;
};

// ------------------------------------------------------------------------
// Ray Tracing Shader Class
// ------------------------------------------------------------------------

/**
 * @brief Ray tracing shader program.
 */
class RayTracingShader : public BaseShader<RayTracingShader>
{
public:
    static constexpr bool ENABLE_UNIFORM_CACHING = true;

    enum class RTShaderType : std::uint8_t
    {
        RayGeneration,
        ClosestHit,
        AnyHit,
        Miss,
        Intersection,
        Callable
    };

    constexpr RayTracingShader() = default;

    explicit RayTracingShader(std::string_view filepath);

    void TraceRays(GLuint width, GLuint height, GLuint depth = 1) const noexcept;

private:
    struct RTShaderSources
    {
        std::string rayGen;
        std::string closestHit;
        std::string anyHit;
        std::string miss;
        std::string intersection;
        std::string callable;
    };

    [[nodiscard]] static RTShaderSources ParseRTShaders(std::string_view filepath);
    [[nodiscard]] GLuint CreateProgram(const RTShaderSources& sources) noexcept;
};

// ------------------------------------------------------------------------
// Concept for shader types
// ------------------------------------------------------------------------

template<typename T>
concept ShaderType = requires(T shader, std::string_view name, float value) {
    shader.Bind();
    shader.Unbind();
    shader.IsValid();
    shader.GetShaderID();
    shader.SetFloat(name, value);
    T::ENABLE_UNIFORM_CACHING;
};

// ------------------------------------------------------------------------
// Legacy wrapper (simplified, no polymorphism)
// ------------------------------------------------------------------------
/**
 * @brief Legacy shader wrapper.
 * @deprecated Use specific shader types instead.
 */
class [[deprecated("Use GraphicsShader, ComputeShader, or RayTracingShader instead")]] Shader
{
public:
    enum class Type : std::uint8_t { Graphics, Compute, RayTracing };

    constexpr Shader() = default;
    explicit Shader(std::string_view filepath);

    void Bind() const noexcept;
    static void Unbind() noexcept;
    [[nodiscard]] GLuint GetShaderID() const noexcept;
    [[nodiscard]] bool IsValid() const noexcept;
    [[nodiscard]] constexpr Type GetType() const noexcept { return m_type; }

    // Uniform setters
    void SetBool(std::string_view name, bool value) const noexcept;
    void SetInt(std::string_view name, int value) const noexcept;
    void SetFloat(std::string_view name, float value) const noexcept;
    void SetVec2(std::string_view name, const glm::vec2& value) const noexcept;
    void SetVec3(std::string_view name, const glm::vec3& value) const noexcept;
    void SetVec4(std::string_view name, const glm::vec4& value) const noexcept;
    void SetMat4(std::string_view name, const glm::mat4& mat) const noexcept;

    // Type-specific operations
    void Dispatch(GLuint x, GLuint y = 1, GLuint z = 1) const noexcept;
    void TraceRays(GLuint width, GLuint height, GLuint depth = 1) const noexcept;

private:
    Type m_type = Type::Graphics;

    // Union to avoid dynamic allocation
    union ShaderStorage
    {
        constexpr ShaderStorage() : graphics{} {}
        ~ShaderStorage() {} // Handled by Shader destructor

        GraphicsShader graphics;
        ComputeShader compute;
        RayTracingShader rayTracing;
    } m_shader;

    void DestroyShader() noexcept;
};