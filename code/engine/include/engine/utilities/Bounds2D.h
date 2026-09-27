#pragma once

#include "Vector3.h"
#include "Vector2.h"
#include "Math.h"

namespace EisEngine {

    /// Defines the boundaries of a rectangle covering a given area.
    struct Bounds2D {
        /// Creates a new 2D bounds object, with bounds defined in clockwise order from the top.
        /// @param topBound: float - the upper y coordinate of the bounded area
        /// @param rightBound: float - the upper x coordinate of the bounded area
        /// @param bottomBound: float - the lower y coordinate of the bounded area
        /// @param leftBound: float - the lower x coordinate of the bounded area
        explicit Bounds2D(float topBound = 0.5f,
                          float rightBound = 0.5f,
                          float bottomBound = -0.5f,
                          float leftBound = -0.5f) :
                top(topBound),
                right(rightBound),
                bottom(bottomBound),
                left(leftBound) {
            ReevaluateSize();
        }

        /// Creates a new 2D bounds object, with bounds defined in clockwise order from the top.
        /// @param bounds: float[4] - the four coordinates defining the boundaries in clockwise order from the top (top-right-bottom-left)
        explicit Bounds2D(std::array<float, 4>& bounds) : 
                top(bounds[0]),
                right(bounds[1]),
                bottom(bounds[2]),
                left(bounds[3]) {
            ReevaluateSize();
        }

        /// the top boundary of the box shape (y-axis).
        float top;
        /// the right boundary of the box shape (x-axis).
        float right;
        /// the bottom boundary of the box shape (y-axis).
        float bottom;
        /// the left boundary of the box shape (x-axis).
        float left;

        /// the size of the box shape.
        Vector2 size;

        /// normalizes the bounds.
        Bounds2D& normalize(const Vector3& scale);

        /// classic 1x1 square bounds.
        static const Bounds2D Square1x1;

        /// Recalculates the size vectors for the bounds.
        void ReevaluateSize() { size = Vector2(Math::Dist(left, right), Math::Dist(top, bottom));}

        /// Determines whether a point is within the bounds or not.
        [[nodiscard]] bool Contains(const Vector3& v) const { return (v.x >= left) && (v.x <= right) && (v.y <= top) && (v.y >= bottom);}

        #pragma region operators

        Bounds2D& operator *= (const Vector3& v);

        operator std::string() const {
            return "Bounds: (top) " + std::to_string(top) +
                ", (right) " + std::to_string(right) +
                ", (bottom) " + std::to_string(bottom) +
                ", (left) " + std::to_string(left) +
                " -> size: " + (std::string)size;
        }
        #pragma endregion
    };
}
