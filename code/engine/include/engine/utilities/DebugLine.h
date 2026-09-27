#pragma once

#include "engine/Utilities.h"

namespace EisEngine {
    class Game;
    namespace ecs { class Entity;}
    namespace components {
        class Transform;
        class Line;
    }

    /// Draws a line between two given points in world space, in the given color.
    class DebugLine {
        using Entity = ecs::Entity;
        using Transform = components::Transform;
    public:
        /// Creates a new DebugLine object.
        /// @param startPoint - Vector3: The local position of where the line should start.
        /// @param endPoint - Vector3: The local position of where the line should end.
        /// @param color - Color: The line's color on screen.
        DebugLine(Game &engine, const Vector3& startPoint, const Vector3& endPoint, const Color &color);

        /// Sets new line positions.
        void UpdateLinePosition(const Vector3& startPoint, const Vector3& endPoint);
        /// Sets a new color for the line.
        void UpdateColor(const Color& color);

        /// The transform for the line.
        Transform* transform;
        /// A function called when an object is intentionally deleted.
        void Invalidate();
    private:
        /// The entity bundling all the lines together.
        Entity* entity;
        /// A pointer to the engine instance.
        Game* engine = nullptr;
    };
}
