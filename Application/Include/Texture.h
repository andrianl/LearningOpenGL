#pragma once

#include <GL/glew.h>
#include <string>
#include "stb_image.h"

class Texture
{
public:
    // Constructor
    // This constructor initializes the texture by loading an image from the specified file path,
    // generating an OpenGL texture, and setting up texture parameters.
    //
    // Parameters:
    // - path: The file path to the image to be loaded as a texture.
    Texture(const std::string& path);

    // Destructor
    // This destructor cleans up resources by deleting the OpenGL texture when the Texture object goes out of scope.
    ~Texture();

    // Bind the texture
    // This method binds the texture, making it the current active texture used for rendering.
    void Bind() const;

    // Unbind the texture
    // This method unbinds the texture, making it inactive.
    void Unbind() const;

    // Getter for texture ID
    // This inline method returns the OpenGL ID of the texture.
    //
    // Returns:
    // - The unique ID of the texture as a GLuint.
    inline const GLuint GetTextureID() const { return TextureID; }

    // Getter for width
    // This inline method returns the width of the texture in pixels.
    //
    // Returns:
    // - The width of the texture as an integer.
    inline const int GetWidth() const { return width; }

    // Getter for height
    // This inline method returns the height of the texture in pixels.
    //
    // Returns:
    // - The height of the texture as an integer.
    inline const int GetHeight() const { return height; }

    // Getter for number of channels
    // This inline method returns the number of color channels in the texture.
    //
    // Returns:
    // - The number of color channels as an integer.
    inline const int GetChannelsNumber() const { return nrChannels; }

private:
    GLuint TextureID;               // OpenGL ID for the texture
    int width, height, nrChannels;  // Width, height, and number of channels of the texture

    // Function to load image using your preferred image loading library (e.g., stb_image)
    // This private method loads an image from the specified file path using stb_image or another image loading library.
    //
    // Parameters:
    // - path: The file path to the image to be loaded.
    // - width: Pointer to an integer where the width of the loaded image will be stored.
    // - height: Pointer to an integer where the height of the loaded image will be stored.
    // - nrChannels: Pointer to an integer where the number of color channels will be stored.
    //
    // Returns:
    // - A pointer to the loaded image data.
    unsigned char* LoadImage(const std::string& path, int* width, int* height, int* nrChannels);

    // Function to free image memory
    // This private method frees the memory used by the loaded image data.
    //
    // Parameters:
    // - data: Pointer to the image data to be freed.
    void FreeImage(unsigned char* data);
};
