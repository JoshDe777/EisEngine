#pragma once

#include <OpenGL/OpenGlInclude.h>

namespace EisEngine {
    class ResourceManager;
    class Cubemap {
        friend ResourceManager;
    public:
        void Bind() const;
    private:
        /// Creates a new Cubemap object
        Cubemap();

        /// Generate cubemap data from source
        void Generate(unsigned int& index, unsigned int width, unsigned int height, unsigned char* data);
        /// Set the rendering parameters in the OpenGL texture object
        void SetParams() const;
        /// The texture ID in the rendering storage system.
        unsigned int textureID;

        /// The width of the cubemap's individual textures.
        int width = -1;
        /// The height of the cubemap's individual textures.
        int height = -1;

        /// texture object format.
        unsigned int internalFormat;
        /// source image format.
        unsigned int imageFormat;

        /// Wrapping mode on s-Axis.
        unsigned int wrapS;
        /// Wrapping mode on t-Axis.
        unsigned int wrapT;
        /// Wrapping mode on r-Axis.
        unsigned int wrapR;
        /// Filtering mode if texture can be fully displayed on screen.
        unsigned int minFilterMode;
        /// Filtering mode if texture cannot be fully displayed on screen (n(texture.pixels) > n(screen.pixels)
        unsigned int maxFilterMode;
    };
}
