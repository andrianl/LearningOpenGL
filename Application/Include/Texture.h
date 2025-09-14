#pragma once
#include <GL/glew.h>
#include <string>
#include "stb_image.h"
#include <Platform.h>

class GraphicsShader;

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

	// Bind the texture to a specific slot
    // This method binds the texture, making it the current active texture used for rendering.
    void Bind(const uint8 Slot) const;

    // Unbind the texture
    // This method unbinds the texture, making it inactive.
    void Unbind() const;

    /**
* @brief Binds a texture to a specific texture unit and sets the corresponding uniform in the shader.
*
* This function activates the specified texture unit, binds the given texture to it, and then updates
* the shader's sampler uniform to point to that texture unit.
*
* @param texture The texture to be bound.
* @param unit The texture unit index (e.g., 0 for GL_TEXTURE0, 1 for GL_TEXTURE1, etc.).
* @param shader The shader program in which the uniform will be updated.
* @param uniformName The name of the sampler uniform in the shader.
*/
    void BindTextureToShader(GLuint unit, const GraphicsShader& shader, std::string_view uniformName) const;

    // Getter for texture ID
    // This inline method returns the OpenGL ID of the texture.
    //
    // Returns:
    // - The unique ID of the texture as a GLuint.
    inline const GLuint GetTextureID() const { return Data.ImageData.TextureID; }

    // Getter for width
    // This inline method returns the width of the texture in pixels.
    //
    // Returns:
    // - The width of the texture as an integer.
	inline const uint16 GetWidth() const
	{
		return Data.ImageData.Packed & 0x3FFF; // 14 біт
	}


    // Getter for height
    // This inline method returns the height of the texture in pixels.
    //
    // Returns:
    // - The height of the texture as an integer.
	inline const uint16_t GetHeight() const
	{
		return (Data.ImageData.Packed >> 14) & 0x3FFF;
	}



    // Getter for number of channels
    // This inline method returns the number of color channels in the texture.
    //
    // Returns:
    // - The number of color channels as an integer.
	inline const uint8 GetChannels() const
	{
		return (Data.ImageData.Packed >> 28) & 0xF; // останні 4 біти
	}


private:
	union ImageInfo
	{
		uint64_t Raw;

		struct Image
		{
			GLuint TextureID;
			uint32 Packed : 32; // width + height + channels
		} ImageData;
	};

    ImageInfo Data;

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

	inline void SetWidth(uint16 InWidth)
	{
        Data.ImageData.Packed = (Data.ImageData.Packed & ~0x3FFF) | (InWidth & 0x3FFF);
	}

	inline void SetHeight(uint16 InHeight)
	{
        Data.ImageData.Packed = (Data.ImageData.Packed & ~(0x3FFF << 14)) | ((InHeight & 0x3FFF) << 14);
	}

	inline void SetChannels(uint8 InChannels)
	{
        Data.ImageData.Packed = (Data.ImageData.Packed & ~(0xF << 28)) | ((InChannels & 0xF) << 28);
	}
};
