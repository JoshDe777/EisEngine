#pragma once

#include <filesystem>
#include <map>
#include "engine/utilities/rendering/Texture2D.h"
#include "engine/utilities/rendering/Shader.h"

#define GET_STRING(x) #x
#define GET_DIR(x) GET_STRING(x)

namespace fs = std::filesystem;

namespace EisEngine {
    inline fs::path resolveAssetPath(const fs::path &relativeAssetPath) {
        auto mergedPath = (GET_DIR(ASSET_ROOT) / relativeAssetPath).make_preferred();
        return fs::canonical(mergedPath);
    }

    /// \n Struct used to debug shader creation.
    struct glStatusData {
        /// \n Indicates whether the shader creation was successful.
        int success;
        /// \n The name given to the shader.
        const char *shaderName;
        /// \n Log messages attributed to the current shader.
        char infoLog[GL_INFO_LOG_LENGTH];
    };

    using Shader = rendering::Shader;

    /// \n Manages files associated with the engine.
    class ResourceManager {
        friend class Game;
    public:
        /// \n Generates a texture from the given file.
        /// @param imagePath - fs::path: the absolute path from the assets folder to the desired file.
        /// @param alpha - bool: Determines whether the texture has an alpha channel or not
        /// \n (e.g. x.png = true, x.jpg = false)
        /// @param textureName - std::string: the name of the given texture. Must be unique!
        static Texture2D* GenerateTextureFromFile(const fs::path& imagePath, const std::string& textureName);

        /// \n fetches a texture using its name.
        static Texture2D* GetTexture(const std::string& name);

        static Shader* GetShader(const std::string& name);

        /// \n Generates a shader program from the given file.
        /// @param vertexShaderPath - std::filesystem::path: the relative path from the assets folder
        /// to the vertex shader definition.
        /// @param fragmentShaderPath - std::filesystem::path: the relative path from the assets folder
        /// to the fragment shader definition.
        /// @param shaderName - std::string: the UNIQUE name for the shader.
        static Shader* GenerateShaderFromFiles(const fs::path& vertexShaderPath,
                                               const fs::path& fragmentShaderPath,
                                               const std::string& shaderName);
    private:
        /// \n Inaccessible constructor. All functions should be used as static members.
        ResourceManager() { }
        /// \n Clears all textures.
        static void Clear();

        /// \n A dictionary of textures associated to their file name.
        static std::map<std::string, std::unique_ptr<Texture2D>> Textures;
        /// \n A dictionary of shaders associated to their name.
        static std::map<std::string, std::unique_ptr<Shader>> Shaders;

        /// \n Compiles a file path to a std::string.
        static std::string ReadText(const fs::path& path);

        /// \n loads a texture from a file.
        /// @param filePath - std::filesystem::path: The file path to the image.
        /// @param alpha - bool: Determines whether the texture has an alpha channel or not.
        static Texture2D loadTextureFromFile(const fs::path& filePath);

        /// \n Creates a shader program of the given shaderType with a definition at the given shader path.
        static unsigned int loadAndCompileShader(GLuint shaderType, const fs::path& filePath);
    };
}
