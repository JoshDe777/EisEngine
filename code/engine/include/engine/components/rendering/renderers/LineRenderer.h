#pragma once

#include "Renderer.h"

namespace EisEngine::components {
    /// \n Contains rendering data for lines.
    /// \n Requires a Line component for proper use.
    class LineRenderer : public Renderer {
    public:
        /// \n Creates a line renderer.
        /// @param color - a 4D vector representing the line's color in RGBA%.
        LineRenderer(Game &engine, guid_t owner, Color color = Color::white) : Renderer(engine, owner, color) { }
    };
}
