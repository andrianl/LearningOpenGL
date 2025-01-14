#include "Texture.h"

#include "stb_image.h"
#include <iostream>

Texture::Texture(const std::string& path)
{
    glGenTextures(1, &TextureID);
    glBindTexture(GL_TEXTURE_2D, TextureID);

    // Set texture wrapping and filtering options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

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