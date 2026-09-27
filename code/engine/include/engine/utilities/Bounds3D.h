#pragma once

#include "engine/utilities/Vector2.h"
#include "engine/utilities/Vector3.h"
#include <array>

namespace EisEngine {
    namespace utilities {
        /// Defines the boundaries of a rectangular prism occupying a 3D area
        class Bounds3D {
        public:

            /// Creates a new 3D bounds object.
            /// @param xMin: float - the lower x coordinate of the bounded area.
            /// @param xMax: float - the upper x coordinate of the bounded area.
            /// @param yMin: float - the lower y coordinate of the bounded area.
            /// @param yMax: float - the upper y coordinate of the bounded area.
            /// @param zMin: float - the lower z coordinate of the bounded area.
            /// @param zMax: float - the upper z coordinate of the bounded area.
            explicit Bounds3D(
                float xMin,
                float xMax,
                float yMin,
                float yMax,
                float zMin,
                float zMax
            );

            /// Creates a new 3D bounds object, with bounds defined in clockwise order from the top.
            /// @param bounds: float[6] - the six coordinates defining the boundaries in pairwise (min, max) order x, y, z.
            explicit Bounds3D(std::array<float, 6> bounds);
            /// Creates a new 3D bounds object, spanning a precise point.
            /// @param bounds: Vector3 - the x, y, z bounds of the object.
            explicit Bounds3D(const Vector3& bounds);

            /// Merge two 3D bounds together into one.
            /// @param A: Bounds3D& - A reference to the first area.
            /// @param B: Bounds3D& - A reference to the second area.
            static Bounds3D Merge(Bounds3D& A, Bounds3D& B);
            /// Expand the bounds to include a specific point.
            /// @param A: Bounds3D& - A reference to the existing area.
            /// @param B: Vector3& - A reference to the point to be included.
            static Bounds3D Expand(Bounds3D& A, const Vector3& B);

            /// Expand the bounds to include a specific point.
            /// @param B: Vector3& - A reference to the point to be included.
            Bounds3D Expand(const Vector3& B);
            /// Get the point describing the centre of the bounded area.
            Vector3 Centre() const;

            /// The limits of the object's stretch in x direction.
            Vector2 xBounds;
            /// The limits of the object's stretch in y direction.
            Vector2 yBounds;
            /// The limits of the object's stretch in z direction.
            Vector2 zBounds;

            /// Get the closest point in or on the edge of the bounded area to the given vector.
            [[nodiscard]] Vector3 GetClosestPointTo(const Vector3& pos) const;
            /// Returns the distance between the points described by the min and max vectors of the bounded area.
            [[nodiscard]] float diagonalSize() const;
        };
    }
}
