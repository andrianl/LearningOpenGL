#pragma once

#include <GL/glew.h>
#include <string>
#include <string_view>
#include "glm/glm.hpp"

/**
 * @brief Enumeration of supported shader stages.
 *
 * This enum lists all shader types recognized by the engine.
 * Only the first set (up to Pixel) is supported by OpenGL in this implementation.
 */
enum class ShaderType : int8_t
{
    None,       ///< No shader type selected
    Compute,    ///< Compute shader
    Vertex,     ///< Vertex shader
    Hull,       ///< Tessellation Control shader
    Domain,     ///< Tessellation Evaluation shader
    Geometry,   ///< Geometry shader
    Pixel,      ///< Fragment (Pixel) shader

    /* Shader types below are not supported by OpenGL in this implementation */

    Amplification,
    Mesh,
    AllGraphics,

    RayGeneration,
    AnyHit,
    ClosestHit,
    Miss,
    Intersection,
    Callable,
    AllRayTracing,

    All
};

/**
 * @brief Holds source code for all supported OpenGL shader stages.
 *
 * Each string contains the GLSL source code for the corresponding shader stage.
 * Empty strings indicate that the stage is not used in the current shader program.
 */
struct ShaderSourceProgram
{
    std::string ComputeShader;   ///< GLSL source for compute shader
    std::string VertexShader;    ///< GLSL source for vertex shader
    std::string HullShader;      ///< GLSL source for tessellation control shader
    std::string DomainShader;    ///< GLSL source for tessellation evaluation shader
    std::string GeometryShader;  ///< GLSL source for geometry shader
    std::string PixelShader;     ///< GLSL source for fragment (pixel) shader
};

/**
 * @brief Encapsulates an OpenGL shader program.
 *
 * The Shader class provides functionality for:
 * - Loading shader source code from a file
 * - Parsing and separating multiple shader stages from a single file
 * - Compiling and linking shader stages into a program
 * - Binding the program for rendering
 * - Setting uniform variables in the shader
 */
class Shader
{
public:
    /**
     * @brief Default constructor.
     *
     * Creates an uninitialized Shader object with no program loaded.
     */
    Shader() = default;

    /**
     * @brief Constructs a Shader by loading and compiling shaders from a file.
     *
     * The file may contain multiple shader stages separated by `#shader <type>` markers.
     *
     * @param filepath Path to the shader source file.
     */
    explicit Shader(std::string_view filepath);

    /**
     * @brief Destructor.
     *
     * Deletes the OpenGL shader program when the Shader object is destroyed.
     */
    ~Shader();

    /// Copy constructor (default)
    Shader(const Shader&) = default;

    /// Copy assignment operator (default)
    Shader& operator=(const Shader&) = default;

    /**
     * @brief Activates this shader program for rendering.
     *
     * Calls glUseProgram with this shader's program ID.
     */
    void Bind() const;

    /**
     * @brief Returns the OpenGL program ID for this shader.
     * @return GLuint representing the compiled and linked shader program.
     */
    inline const GLuint GetShaderID() const { return ShaderID; }

    // ------------------------------------------------------------------------
    // Uniform Setter Functions
    // ------------------------------------------------------------------------

    /**
     * @brief Retrieves the location of a uniform variable in the shader program.
     *
     * @param name Name of the uniform variable.
     * @return GLint location of the uniform, or -1 if not found.
     */
    GLint GetUniformLocation(std::string_view name) const;

    /// Sets a boolean uniform variable.
    void SetBool(std::string_view name, bool value) const;

    /// Sets an integer uniform variable.
    void SetInt(std::string_view name, int value) const;

    /// Sets a float uniform variable.
    void SetFloat(std::string_view name, float value) const;

    /// Sets a vec2 uniform variable from a glm::vec2.
    void SetVec2(std::string_view name, const glm::vec2& value) const;

    /// Sets a vec2 uniform variable from two floats.
    void SetVec2(std::string_view name, float x, float y) const;

    /// Sets a vec3 uniform variable from a glm::vec3.
    void SetVec3(std::string_view name, const glm::vec3& value) const;

    /// Sets a vec3 uniform variable from three floats.
    void SetVec3(std::string_view name, float x, float y, float z) const;

    /// Sets a vec4 uniform variable from a glm::vec4.
    void SetVec4(std::string_view name, const glm::vec4& value) const;

    /// Sets a vec4 uniform variable from four floats.
    void SetVec4(std::string_view name, float x, float y, float z, float w) const;

    /// Sets a mat2 uniform variable.
    void SetMat2(std::string_view name, const glm::mat2& mat) const;

    /// Sets a mat3 uniform variable.
    void SetMat3(std::string_view name, const glm::mat3& mat) const;

    /// Sets a mat4 uniform variable.
    void SetMat4(std::string_view name, const glm::mat4& mat) const;

private:
    /**
     * @brief Parses a shader source file into separate shader stage strings.
     *
     * The file should contain sections marked with `#shader <type>` where `<type>` is
     * one of: compute, vertex, hull, domain, geometry, pixel/fragment.
     *
     * @param filepath Path to the shader source file.
     * @return ShaderSourceProgram struct containing the separated source code.
     */
    ShaderSourceProgram ParseShader(const std::string& filepath);

    /**
     * @brief Creates and links an OpenGL shader program from provided source code.
     *
     * Only non-empty shader stage strings will be compiled and attached.
     *
     * @param Source Struct containing GLSL source for each shader stage.
     * @return GLuint ID of the created shader program.
     */
    GLuint CreateShader(const ShaderSourceProgram& Source);

    /**
     * @brief Compiles a single shader stage from source code.
     *
     * @param Source GLSL source code for the shader.
     * @param Type OpenGL shader type enum (e.g., GL_VERTEX_SHADER).
     * @return GLuint ID of the compiled shader object, or 0 on failure.
     */
    GLuint CompileShader(const std::string& Source, GLuint Type);

    GLuint ShaderID = 0; ///< OpenGL program object ID for this shader.
};