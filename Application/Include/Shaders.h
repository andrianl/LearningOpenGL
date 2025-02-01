#pragma once

#include <GL/glew.h>
#include <string_view>
#include "glm/glm.hpp"

/**
 * @brief Struct to hold the vertex and pixel shader source code.
 */
struct ShaderSourceProgram
{
    std::string VertexShader;  ///< Vertex shader source code.
    std::string PixelShader;   ///< Pixel (fragment) shader source code.
};

/**
 * @brief The Shader class encapsulates an OpenGL shader program.
 *
 * This class provides functionality for loading, compiling, and linking shaders,
 * as well as setting uniform variables in the shader program.
 */
class Shader
{
public:
    /**
     * @brief Default constructor.
     *        Useful for creating an uninitialized Shader object.
     */
    Shader() = default;

    /**
     * @brief Constructs a Shader object by loading and compiling shaders from a file.
     * @param filepath The path to the file containing both vertex and pixel shader source code.
     */
    explicit Shader(std::string_view filepath);

    /**
     * @brief Destructor that cleans up OpenGL shader resources.
     */
    ~Shader();

    /**
     * @brief Copy constructor (default).
     */
    Shader(const Shader&) = default;

    /**
     * @brief Copy assignment operator (default).
     */
    Shader& operator=(const Shader&) = default;

    /**
     * @brief Activates the shader program for OpenGL rendering.
     *
     * This method binds the shader program, making it the current shader program used for rendering.
     */
    void Bind() const;

    /**
     * @brief Retrieves the OpenGL shader program ID.
     * @return The unique identifier of the shader program.
     */
    inline const GLuint GetShaderID() const { return ShaderID; }

    // ------------------------------------------------------------------------
    // Uniform Setter Functions (Pascal Case)
    // ------------------------------------------------------------------------
    /**
     * @brief Retrieves the location of a uniform variable in the shader program.
     *
     * This method queries OpenGL for the location of the specified uniform variable within the shader program.
     *
     * @param name The name of the uniform variable.
     * @return The location of the uniform variable as an GLint. Returns -1 if the uniform is not found.
     */
    GLint GetUniformLocation(std::string_view name) const;

    /**
     * @brief Sets a boolean uniform variable in the shader.
     * @param name The name of the uniform variable.
     * @param value The boolean value to set.
     */
    void SetBool(std::string_view name, bool value) const;

    /**
     * @brief Sets an integer uniform variable in the shader.
     * @param name The name of the uniform variable.
     * @param value The integer value to set.
     */
    void SetInt(std::string_view name, int value) const;

    /**
     * @brief Sets a float uniform variable in the shader.
     * @param name The name of the uniform variable.
     * @param value The float value to set.
     */
    void SetFloat(std::string_view name, float value) const;

    /**
     * @brief Sets a vec2 uniform variable in the shader.
     * @param name The name of the uniform variable.
     * @param value The glm::vec2 value to set.
     */
    void SetVec2(std::string_view name, const glm::vec2& value) const;

    /**
     * @brief Sets a vec2 uniform variable in the shader using individual components.
     * @param name The name of the uniform variable.
     * @param x The x-component of the vector.
     * @param y The y-component of the vector.
     */
    void SetVec2(std::string_view name, float x, float y) const;

    /**
     * @brief Sets a vec3 uniform variable in the shader.
     * @param name The name of the uniform variable.
     * @param value The glm::vec3 value to set.
     */
    void SetVec3(std::string_view name, const glm::vec3& value) const;

    /**
     * @brief Sets a vec3 uniform variable in the shader using individual components.
     * @param name The name of the uniform variable.
     * @param x The x-component of the vector.
     * @param y The y-component of the vector.
     * @param z The z-component of the vector.
     */
    void SetVec3(std::string_view name, float x, float y, float z) const;

    /**
     * @brief Sets a vec4 uniform variable in the shader.
     * @param name The name of the uniform variable.
     * @param value The glm::vec4 value to set.
     */
    void SetVec4(std::string_view name, const glm::vec4& value) const;

    /**
     * @brief Sets a vec4 uniform variable in the shader using individual components.
     * @param name The name of the uniform variable.
     * @param x The x-component of the vector.
     * @param y The y-component of the vector.
     * @param z The z-component of the vector.
     * @param w The w-component of the vector.
     */
    void SetVec4(std::string_view name, float x, float y, float z, float w) const;

    /**
     * @brief Sets a mat2 uniform variable in the shader.
     * @param name The name of the uniform variable.
     * @param mat The glm::mat2 matrix to set.
     */
    void SetMat2(std::string_view name, const glm::mat2& mat) const;

    /**
     * @brief Sets a mat3 uniform variable in the shader.
     * @param name The name of the uniform variable.
     * @param mat The glm::mat3 matrix to set.
     */
    void SetMat3(std::string_view name, const glm::mat3& mat) const;

    /**
     * @brief Sets a mat4 uniform variable in the shader.
     * @param name The name of the uniform variable.
     * @param mat The glm::mat4 matrix to set.
     */
    void SetMat4(std::string_view name, const glm::mat4& mat) const;

private:
    /**
     * @brief Parses the shader source file and separates vertex and pixel shader code.
     * @param filepath The path to the shader source file.
     * @return A ShaderSourceProgram struct containing both vertex and pixel shader code.
     */
    ShaderSourceProgram ParseShader(const std::string& filepath);

    /**
     * @brief Creates and links a shader program using vertex and pixel shader source code.
     * @param VertexShader The vertex shader source code.
     * @param PixelShader The pixel (fragment) shader source code.
     * @return The unique ID of the created shader program.
     */
    GLuint CreateShader(const std::string& VertexShader, const std::string& PixelShader);

    /**
     * @brief Compiles a shader from source code.
     * @param Source The source code of the shader.
     * @param Type The type of shader (GL_VERTEX_SHADER or GL_FRAGMENT_SHADER).
     * @return The unique ID of the compiled shader.
     */
    GLuint CompileShader(const std::string& Source, GLuint Type);

    GLuint ShaderID = 0;  ///< OpenGL ID for the shader program.
};
