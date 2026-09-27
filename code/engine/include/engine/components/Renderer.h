#pragma once

#include "engine/ecs/Component.h"
#include "engine/utilities/rendering/Shader.h"
#include "engine/utilities/rendering/Material.h"
#include "engine/utilities/rendering/Texture2D.h"


namespace EisEngine {
    using ecs::Component;
    using rendering::Shader;

    namespace components {
        /// Contains rendering data for any type of mesh.
        class Renderer : public Component {
            friend class ecs::Entity;
        public:
            /// Creates a renderer.
            /// @param diffTex - Texture2D*: A pointer to a texture item. Can be null.
            /// @param mat - Material*: A pointer to a material item. Can be null.
            /// @param layer - std::string: The rendering layer for meshes paired with a renderer.
            /// \n Currently supported: {"UI" for UI Elements, and [any other string] for regular rendering}.
            /// @param normMap: Texture2D* - A pointer to a normal map. Can be null.
            Renderer(Game &engine, guid_t owner,
                     Texture2D* diffTex = nullptr,
                     Material* mat = nullptr,
                     std::string  layer = "default",
                     Texture2D* normMap = nullptr);
            Renderer(const Renderer &renderer) = delete;
            Renderer(Renderer &&other) noexcept;

            /// Applies rendering data to the active shader before drawing meshes.
            virtual void ApplyData(Shader& shader);
            /// returns a renderer's rendering layer.
            [[nodiscard]] std::string GetLayer() { return m_layer;}

            /// Sets a new diffuse texture for the corresponding object.
            void SetDiffuseTexture(Texture2D* newTexture) { diffuseTexture = newTexture;}
            /// Sets a new normal map for the corresponding object.
            void SetNormalMap(Texture2D* newTexture) { normalMap = newTexture;}
            /// Returns a pointer to the texture assigned to a renderer.
            Texture2D* GetDiffuseTexture() { return diffuseTexture;}
            /// Returns a pointer to the normal map assigned to a renderer.
            Texture2D* GetNormalMap(){ return normalMap;}

            /// The material attributed to the associated mesh.
            Material* material;
        protected:
            /// the diffuse/albedo texture attributed to the associated mesh.
            Texture2D* diffuseTexture;
            /// the normal map attributed to the associated mesh.
            Texture2D* normalMap;
            /// The rendering layer of the associated mesh.
            std::string m_layer;
        };
    }
}
