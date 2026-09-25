#pragma once

#include "Renderer.h"

namespace EisEngine::components {
    /// \n Contains rendering data for sprites.
    /// \n Requires a SpriteMesh component for proper use.
    class SpriteRenderer : public Renderer {
    public:
        /// \n Creates a sprite renderer.
        /// @param spritePath - std::filesystem::path: the path from the assets folder to the image to be
        /// rendered as a sprite.
        /// @param color - Color: A 4D vector representing a mesh's color in RGBA%.
        /// @param layer - std::string: The rendering layer for meshes paired with a renderer.
        /// \n Currently supported: {"UI" for UI Elements, and [any other string] for regular rendering}.
        explicit SpriteRenderer(Game &engine,
                                guid_t owner,
                                const fs::path& spritePath = "images/ui/blank.png",
                                const Color& color = Color::white,
                                const std::string& layer = "Background");

        /// \n Applies rendering data to the active shader before drawing meshes.
        void ApplyData(Shader& shader) override;
        /// \n Sets a new texture for the corresponding sprite.
        void SetNewTexture(Texture2D* newTexture) {texture = newTexture;}
        /// \n Returns a pointer to the texture assigned to a renderer.
        [[nodiscard]] Texture2D* GetTexture() { return texture;}
    private:
        /// \n A pointer to the texture assigned to this shader.
        Texture2D *texture;
    };
}
