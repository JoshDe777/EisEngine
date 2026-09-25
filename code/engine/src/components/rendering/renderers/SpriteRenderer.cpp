#include "engine/components/rendering/renderers/SpriteRenderer.h"
#include "engine/ResourceManager.h"

namespace EisEngine::components {
    // converts a file path to a regular string.
    std::string ResolveFilename(const fs::path& path){ return path.filename().string();}

    SpriteRenderer::SpriteRenderer(Game &engine, guid_t owner, const fs::path &spritePath,
                                   const Color& color, const std::string& layer) :
            Renderer(engine, owner, color, layer) {
        texture = ResourceManager::GenerateTextureFromFile(spritePath,
                                                           ResolveFilename(spritePath));
    }

    // applies the sprite texture to the active shader.
    void SpriteRenderer::ApplyData(Shader& shader) {
        Renderer::ApplyData(shader);
        Shader::ApplyTexture(*texture);
    }
}
