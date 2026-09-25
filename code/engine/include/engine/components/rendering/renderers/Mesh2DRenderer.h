#pragma once

#include "Renderer.h"

namespace EisEngine::components {
    /// \n Contains rendering data for primitive 2D meshes.
    /// \n Requires a Mesh2D component for proper use.
    class Mesh2DRenderer : public Renderer {
    public:
        /// \n Creates a primitive mesh renderer.
        /// @param color - a 4D vector representing the mesh's color in RGBA%.
        Mesh2DRenderer(Game &engine, guid_t owner, Color color = Color::white) : Renderer(engine, owner, color) { }
    };
}
