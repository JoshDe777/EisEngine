#include "stb_image.h"

#include "engine/ResourceManager.h"
#include "engine/utilities/Debug.h"

namespace EisEngine {
    std::map<std::string, std::unique_ptr<Texture2D>> ResourceManager::Textures = {};
    std::map<std::string, std::unique_ptr<Shader>> ResourceManager::Shaders = {};

    Texture2D* ResourceManager::GenerateTextureFromFile( const fs::path &imagePath, const std::string &textureName) {
        if(Textures[textureName] == nullptr)
            Textures[textureName] = std::make_unique<Texture2D>(
                    loadTextureFromFile(resolveAssetPath(imagePath)));
        return GetTexture(textureName);
    }

    Texture2D *ResourceManager::GetTexture(const std::string &name) {
        if(Textures.empty()){
            DEBUG_WARN("No textures created in resource manager system.")
            return nullptr;
        }
        return Textures[name].get();
    }

    Texture2D ResourceManager::loadTextureFromFile(const fs::path& filePath) {
        std::string pathString = filePath.string();
        const char* filename = pathString.c_str();
        Texture2D texture;

        int width, height, nrChannels;
        unsigned char* data = stbi_load(filename, &width, &height, &nrChannels, 0);

        if(!data){
            DEBUG_ERROR("Failed to load image: " + pathString)
            return texture;
        }

        if(nrChannels == 4) {
            texture.internalFormat = GL_RGBA;
            texture.imageFormat = GL_RGBA;
        }

        texture.Generate(width, height, data);
        
        stbi_image_free(data);

        return texture;
    }

    Shader *ResourceManager::GenerateShaderFromFiles(const fs::path &vertexShaderPath,
                                                     const fs::path &fragmentShaderPath,
                                                     const std::string &shaderName) {
        if(Shaders[shaderName] == nullptr)
            Shaders[shaderName] = std::make_unique<Shader>(
                    loadAndCompileShader(GL_VERTEX_SHADER, vertexShaderPath),
                    loadAndCompileShader(GL_FRAGMENT_SHADER, fragmentShaderPath)
                    );
        return GetShader(shaderName);
    }

    Shader *ResourceManager::GetShader(const std::string &name) {
        if(Shaders.empty()){
            DEBUG_WARN("No shaders created in resource manager system.")
            return nullptr;
        }
        return Shaders[name].get();
    }

    std::string ResourceManager::ReadText(const fs::path &path) {
        std::ifstream sourceFile(resolveAssetPath(path));
        std::stringstream buffer;
        buffer << sourceFile.rdbuf();
        return buffer.str();
    }

    unsigned int ResourceManager::loadAndCompileShader(GLuint shaderType, const fs::path &filePath) {
        auto shaderID = glCreateShader(shaderType);
        auto shaderSource = ReadText(filePath);
        auto source = shaderSource.c_str();
        glShaderSource(shaderID, 1, &source, nullptr);
        glCompileShader(shaderID);

        glStatusData compilationStatus{};
        compilationStatus.shaderName = shaderType == GL_VERTEX_SHADER ? "Vertex" : "Fragment";
        glGetShaderiv(shaderID, GL_COMPILE_STATUS, &compilationStatus.success);
        if(compilationStatus.success == GL_FALSE) {
            glGetShaderInfoLog(shaderID, GL_INFO_LOG_LENGTH, nullptr, compilationStatus.infoLog);
            DEBUG_RUNTIME_ERROR( std::string(compilationStatus.shaderName) + " shader compilation failed.\n" +
            std::string(compilationStatus.infoLog))
        }

        return shaderID;
    }

    void ResourceManager::Clear(){
        for (auto it = Textures.begin(); it != Textures.end(); ++it)
            glDeleteTextures(1, &it->second->textureID);
    }
}