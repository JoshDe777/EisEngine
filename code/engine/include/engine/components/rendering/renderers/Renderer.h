#pragma once

#include "engine/ecs/Component.h"
#include "engine/utilities/rendering/Shader.h"

namespace EisEngine {
    using ecs::Component;
    using rendering::Shader;

    namespace components {
        /// \n *deprecated* Contains rendering data for any type of mesh.
        /// \n Use specialized renderers tailored to a specific mesh type instead.
        class Renderer : public Component {
        public:
            /// \n Creates a renderer.
            /// @param color - Color: A 4D vector representing a mesh's color in RGBA%.
            /// @param layer - std::string: The rendering layer for meshes paired with a renderer.
            /// \n Currently supported: {"UI" for UI Elements, and [any other string] for regular rendering}.
            Renderer(Game &engine, guid_t owner,
                     const Color& color = Color::white, std::string  layer = "Background");
            Renderer(const Renderer &renderer) = delete;
            Renderer(Renderer &&other) noexcept;

            /// \n Applies rendering data to the active shader before drawing meshes.
            virtual void ApplyData(Shader& shader);

            /// \n Sets a renderer's color to the provided value.
            void SetColor(Color newColor) { m_color = newColor;};
            /// \n returns the renderer's color in RGBA%.
            [[nodiscard]] Color GetColor() { return m_color;}
            /// \n returns a renderer's rendering layer.
            [[nodiscard]] std::string GetLayer() { return m_layer;}
        protected:
            /// \n The color given to fragments rendered with this renderer.
            Color m_color;
            /// \n The rendering layer of the associated mesh.
            std::string m_layer;
        };
    }
}
