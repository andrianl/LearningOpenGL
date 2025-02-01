#include "Texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <iostream>
#include <Shaders.h>

Texture::Texture(const std::string& path)
{
    glGenTextures(1, &TextureID);
    glBindTexture(GL_TEXTURE_2D, TextureID);

    // Set texture wrapping and filtering options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_set_flip_vertically_on_load(true);
    // Load image
    unsigned char* data = LoadImage(path, &width, &height, &nrChannels);
    if (data)
    {
        GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        std::cout << "Texture loaded: " << path << std::endl;
    }
    else
    {
        std::cerr << "Failed to load texture: " << path << std::endl;
    }

    // Free image memory
    FreeImage(data);
}

Texture::~Texture()
{
    glDeleteTextures(1, &TextureID);
}

void Texture::Bind() const
{
    glBindTexture(GL_TEXTURE_2D, TextureID);
}

void Texture::Unbind() const
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

unsigned char* Texture::LoadImage(const std::string& path, int* width, int* height, int* nrChannels)
{
    // Use your preferred image loading library here
    // For example, stb_image.h
    // unsigned char* data = stbi_load(path.c_str(), width, height, nrChannels, 0);
    // return data;
    return stbi_load(path.c_str(), width, height, nrChannels, 0);
}

void Texture::FreeImage(unsigned char* data)
{
    // Use your preferred image loading library here
    // For example, stb_image.h
    // stbi_image_free(data);
    stbi_image_free(data);
}

void Texture::BindTextureToShader(GLuint unit, const Shader& shader, std::string_view uniformName) const
{
    // Отримуємо максимальну кількість текстурних блоків.
    static GLint maxTextureUnits = 0;
    if (maxTextureUnits == 0) {
        glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &maxTextureUnits);
    }

    // Перевірка на допустимість текстурного блоку.
    if (unit >= static_cast<GLuint>(maxTextureUnits)) {
        std::cerr << "Error: Texture unit exceeds the maximum supported texture units (" << maxTextureUnits << ")." << std::endl;
        return;
    }

    // Activate the specified texture unit.
    glActiveTexture(GL_TEXTURE0 + unit);

    // Bind the texture to the current active texture unit.
    Bind();

    // Set the sampler uniform in the shader to use the specified texture unit.
    shader.SetInt(uniformName, static_cast<int>(unit));
}
