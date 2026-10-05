#pragma once

#include <OpenGL/OpenGlInclude.h>

namespace EisEngine {
    enum FilterModes{
        NEAREST = 0,
        LINEAR = 1,
        MIPMAP_NEAREST = 2,
        MIPMAP_LINEAR = 3
    };

    class ResourceManager;
    /// A 2-dimensional texture. Is attached to a Renderer component to apply.
    class Texture2D {
        friend ResourceManager;
    public:
        /// Binds the texture as the current active GL_TEXTURE_2D.
        void Bind() const;
        /// Toggles the use of mipmaps. Starts off as true.
        void SetFilteringMode(FilterModes mode);

        /// Texture width in pixels.
        unsigned int Width;
        /// Texture height in pixels.
        unsigned int Height;
    private:
        /// Creates a new texture object.
        Texture2D();

        /// Generates a texture from the given image data.
        /// @param width - unsigned int: the width of the texture in pixels.
        /// @param height - unsigned int: the height of the texture in pixels
        /// @param data - unsigned char*: a pointer to the image data.
        void Generate(unsigned int width, unsigned int height, unsigned char* data);

        /// the unique ID in the resource management system.
        unsigned int textureID;

        /// texture object format.
        unsigned int internalFormat;
        /// source image format.
        unsigned int imageFormat;

        /// Wrapping mode on s-Axis.
        unsigned int wrapS;
        /// Wrapping mode on t-Axis.
        unsigned int wrapT;
        /// Filtering mode if texture can be fully displayed on screen.
        unsigned int minFilterMode;
        /// Filtering mode if texture cannot be fully displayed on screen (n(texture.pixels) > n(screen.pixels)
        unsigned int maxFilterMode;
    };
}
