#pragma once

#include <OpenGL/OpenGlInclude.h>
#include "engine/ecs/System.h"
#include "Camera.h"
#include "engine/utilities/rendering/Shader.h"

namespace EisEngine::systems {
    using namespace rendering;
    /// \n The system drawing objects onto the display.
    class RenderingSystem : public System {
    public:
        /// \n creates an instance of the EisEngine rendering system.
        explicit RenderingSystem(Game& engine);
        /// \n displays meshes of all supported kinds on screen.
        void Draw();
    private:
        /// \n A pointer to the active camera object.
        Camera* camera = nullptr;
        /// \n VAO array storing a VAO for each type of mesh in order: Mesh2D, Line, SpriteMesh, uiMesh.
        std::array<GLuint, 4> VAO;
    };
}
