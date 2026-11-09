#pragma once

#include <unordered_map>
#include <filesystem>
#include <string>

namespace EisEngine::rendering {
    using namespace std;

    enum ShaderType {
        /// Classic lighting model.
        BLINN_PHONG = 0,
        /// Smoother lighting model.
        COOK_TORRANCE = 1,
        /// Cartoonish finite color gradients.
        TOON = 2,
        /// 2D object displayed in screen space.
        SCREEN_UI = 3,
        /// Depth-mapping shader used with glassy shader.
        /// /!\ THIS SHADER DOES NOT HAVE AN INDEPENDENT RENDERING PASS. FOR GLASSY RENDERING USE ShaderType::GLASSY!
        DEPTH = 4,
        /// Translucent object with light distortion properties.
        GLASSY = 5,
        /// Cubemap for 'infinite' rendering.
        SKYBOX = 6
    };

    /// A data structure linking each shader type to its pair of [vertex, fragment] shader programs.
    unordered_map<ShaderType, pair<filesystem::path, filesystem::path>> shader_files = {
        {ShaderType::BLINN_PHONG, {"shaders/vert-shader3D.vert", "shaders/frag-blinn_phong.frag"}},
        {ShaderType::COOK_TORRANCE, {"shaders/vert-shader3D.vert", "shaders/frag-cook_torrance.frag"}},
        {ShaderType::TOON, {"shaders/vert-shader3D.vert", "shaders/frag-toon.frag"}},
        {ShaderType::SCREEN_UI, {"shaders/vert-no_normals.vert", "shaders/frag-sprite_unlit.frag"}},
        {ShaderType::DEPTH, {"shaders/vert-geometry_debug.vert", "shaders/frag-depth_mapping.frag"}},
        {ShaderType::GLASSY, {"shaders/vert-shader3D.vert", "shaders/frag-glassy.frag"}},
        {ShaderType::SKYBOX, {"shaders/vert-skybox.vert", "shaders/frag-skybox.frag"}}
    };

    inline string to_string(const ShaderType& type) {
        switch (type) {
            case BLINN_PHONG: return "Blinn-Phong";
            case COOK_TORRANCE: return "Cook-Torrance";
            case TOON: return "Toon";
            case SCREEN_UI: return "Screen-UI";
            case DEPTH: return "Depth-Mapping";
            case GLASSY: return "Glassy";
            case SKYBOX: return "Skybox";
            default: return "Unsupported";
        }
    }
}
