#pragma once

#include "engine/ecs/Component.h"
#include "engine/utilities/rendering/Shader.h"

namespace EisEngine {
    using ecs::Component;
    using rendering::Shader;

    namespace components {
        /// This component is a Renderer specialized on the rendering of cubemaps, i.e. Skybox.
        class CubemapRenderer: public Component {
        public:
            CubemapRenderer(Game& engine, guid_t owner, Cubemap* tex);
            CubemapRenderer(const CubemapRenderer &renderer) = delete;
            CubemapRenderer(CubemapRenderer &&other) noexcept;

            /// Applies cubemap data to the active shader.
            /// @param shader: Shader& - a reference to the active shader.
            virtual void ApplyData(Shader& shader);
            /// Applies a cubemap texture to the object.
            /// @param newTex: Cubemap* - a pointer to the Cubemap texture used.
            void SetCubemapTexture(Cubemap* newTex) {texture = newTex;}
            /// Returns a pointer to the active cubemap texture.   
            Cubemap* GetTexture() {return texture;}
        protected:
            /// A method called when this object is intentionally deleted.
            void Invalidate() override;
            /// A pointer to the cubemap texture associated with this object.
            Cubemap* texture;                   // 27.09.2026 - shared_ptr reappropriates the object and can lead to lifetime issues.
        };
    }
} // EisEngine
