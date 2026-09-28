#pragma once

#include "engine/utilities/Vector3.h"
#include "engine/utilities/Color.h"

#include <algorithm>

namespace EisEngine {
    namespace rendering{
        class Shader;
    }

    /// A class containing an object's material data. Is attached to a Renderer component to apply.
    class Material {
        using Shader = rendering::Shader;
    public:
        /// Creates a new Material object from the given data.
        /// @param diffuse - Vector3: The base, lit color/tint for the object.\n
        ///     Defaults to white (1,1,1).
        /// @param emission - Vector3: The color of light emitted by the object in all directions.\n
        ///     Defaults to black (0,0,0) - no light emitted.
        /// @param opacity - float: The object's opacity = inverse transparency.\n
        ///     Defaults to 1 (fully opaque).
        /// @param metallic - float: The degree of metallic property the object has. Correlates with gloss/shininess.\n
        ///     Defaults to 0 (not metallic).
        /// @param roughness - float: The inverse surface uniformity - the rougher the surface, the noisier the lighting effects.\n
        ///     Defaults to 0.5 (semi-matte)
        explicit Material(
                std::string  name,
                const Vector3& diffuse = Vector3(1, 1, 1),
                const Vector3& emission = Vector3::zero,
                const float& opacity = 1,
                const float& metallic = 0,
                const float& roughness = 0.5f
            );

        /// Applies material data to the active shader.
        void ApplyMatData(Shader& shader);

        /// Prints out the material's values for debugging purposes.
        void Print() const;

        #pragma region getters
        /// Get the material's diffuse colour in RGB [0, 1].
        const Vector3& GetDiffuse() {return diffuse;}
        /// Get the material's emission colour [0, 1].
        const Vector3& GetEmission() {return emission;}
        /// Get the material's opacity value [0, 1]. 
        [[nodiscard]] const float& GetOpacity() const {return opacity;}
        /// Get the material's metallic value [0, 1].
        [[nodiscard]] const float& GetMetallic() const {return metallic;}
        /// Get the material's roughness value [0, 1].
        [[nodiscard]] const float& GetRoughness() const {return roughness;}
        /// Get the textures' tiling factor. 
        [[nodiscard]] const float& GetTiling() const {return tiling;}
        /// Get the material's emission intensity value.
        [[nodiscard]] const float& GetIntensity() const {return intensity;}
        /// Get the material's unique name.
        const std::string& Name() {return name;}
        #pragma endregion

        #pragma region setters
        /// Set the material's diffuse colour in RGB [0, 1].
        /// Values greater than 1 get clamped to 1, so for RGB [0, 255] divide your values by 255.
        /// @param val: Vector3 - the new colour in RGB [0, 1]
        void SetDiffuse(const Vector3& val) {
            auto x = std::clamp(val.x, 0.0f, 1.0f);
            auto y = std::clamp(val.y, 0.0f, 1.0f);
            auto z = std::clamp(val.z, 0.0f, 1.0f);
            diffuse = Vector3(x, y, z);}
        /// Set the material's diffuse colour in RGB [0, 1].
        /// (Note that transparency val.a is ignored)
        /// @param val: Color - the new colour in RGBA [0, 1]
        void SetDiffuse(const Color& val) {diffuse = Vector3(val.r, val.g, val.b);}
        /// Set the material's emission colour in RGB [0, 1]
        /// Values greater than 1 get clamped to 1, so for RGB [0, 255] divide your values by 255. 
        /// Materials with an emission intensity of 0 will not need this value.
        /// @param val: Vector3 - the new colour in RGB [0, 1]
        void SetEmission(const Vector3& val) {
            auto x = std::clamp(val.x, 0.0f, 1.0f);
            auto y = std::clamp(val.y, 0.0f, 1.0f);
            auto z = std::clamp(val.z, 0.0f, 1.0f);
            emission = Vector3(x, y, z);}
        /// Set the material's opacity value [0, 1]
        /// Opacity = 0 -> object is invisisble; Opacity = 1 -> object is fully intransparent.
        void SetOpacity(const float& val) {opacity = std::clamp(val, 0.0f, 1.0f);}
        /// Set the material's metallic property [0, 1]
        void SetMetallic(const float& val) {metallic = std::clamp(val, 0.0f, 1.0f);}
        /// Set the material's roughness property [0, 1]
        void SetRoughness(const float& val) {roughness = std::clamp(val, 0.0f, 1.0f);}
        /// Set the material's tiling property. Materials without any textures will ignore this value.
        void SetTiling(const float& val){tiling = val;}
        /// Set the material's emission intensity. Materials with an emission colour of black (0, 0, 0) will not utilize this property.
        void SetIntensity(const float& val) {intensity = val;}
        #pragma endregion
    private:
        /// The material's diffuse colour. Defaults to white (1, 1, 1).
        Vector3 diffuse;
        /// The material's emission colour. Defaults to black (0, 0, 0).
        Vector3 emission;
        /// The material's opacity property. Defaults to 1 (fully opaque).
        float opacity;
        /// The material's metallic property. Defaults to 0.
        float metallic;
        /// The material's roughness property. Defaults to 0,5.
        float roughness;
        /// The material's tiling property. Defaults to 1 (no tiling).
        float tiling = 1.0f;
        /// The material's emission intensity property. Defaults to 0 (no emission).
        float intensity = 0.0f;
        /// The material's unique name.
        std::string name;
    };
}

using Material = EisEngine::Material;
