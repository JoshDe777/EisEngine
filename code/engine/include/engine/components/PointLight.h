#pragma once

#include "engine/ecs/Component.h"
#include "engine/utilities/Vector3.h"
#include "engine/utilities/rendering/Material.h"

namespace EisEngine{
    namespace rendering { class Shader; }
    namespace systems { class LightSystem; }
    namespace components {
        using Component = EisEngine::ecs::Component;
        using Shader = EisEngine::rendering::Shader;

        /// This component represents a point light source, emitting its light evenly in all possible directions.
        class PointLight : public Component {
            friend class EisEngine::systems::LightSystem;               // management
        public:
            /// Creates a new Point Light component.
            /// @param mat: Material* - a pointer to a material containing its data (light colour, intensity...)
            explicit PointLight (
                    Game& game, guid_t owner,
                    Material* mat
            );
            PointLight(const PointLight &light) = delete;
            PointLight(PointLight &&other) noexcept;

            /// Applies the light properties to the active shader.
            /// @param shader: Shader& - a reference to the active shader
            /// @param index: int& - 
            void Apply(Shader& shader, const int& index) const;

            /// Returns the light's emission colour (Vector3).
            Vector3 GetEmission() const { return mat->GetEmission();}
            /// Returns the light's emission intensity (float).
            float GetIntensity() const {return mat->GetIntensity();}
            /// Gets the light's position in world space (Vector3).
            Vector3 position() const;

            /// Sets the light's emission colour.
            /// @param v: Vector3& - the new light's colour.
            void SetEmission(const Vector3& v) { mat->SetEmission(v);}
            /// Sets the light's intensity/strength.
            /// @param I: float& - the new intensity value.
            void SetIntensity(const float& I) { mat->SetIntensity(I);}

            /// A pointer to the light's material.
            Material* mat;
        protected:
            /// A function called once when the object is intentionally deleted.
            virtual void Invalidate() override;
        private:
            /// A reference to the engine object for.
            Game& engine;
        };
    }
}
