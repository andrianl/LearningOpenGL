#pragma once

#include "stb_image.h"

#include <GL/glew.h>
#include <string>
#include <iostream>

class Texture
{
public:
    // Constructor
    Texture(const std::string& path)
    {
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);

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

    // Destructor
    ~Texture() { glDeleteTextures(1, &textureID); }

    // Bind the texture
    void Bind() const { glBindTexture(GL_TEXTURE_2D, textureID); }

    // Unbind the texture
    void Unbind() const { glBindTexture(GL_TEXTURE_2D, 0); }

private:
    GLuint textureID;
    int width, height, nrChannels;

    // Function to load image using your preferred image loading library (e.g., stb_image)
    unsigned char* LoadImage(const std::string& path, int* width, int* height, int* nrChannels)
    {
        // Use your preferred image loading library here
        // For example, stb_image.h
        // unsigned char* data = stbi_load(path.c_str(), width, height, nrChannels, 0);
        // return data;
        return stbi_load(path.c_str(), width, height, nrChannels, 0);
    }

    // Function to free image memory
    void FreeImage(unsigned char* data)
    {
        // Use your preferred image loading library here
        // For example, stb_image.h
        // stbi_image_free(data);
        stbi_image_free(data);
    }
};
