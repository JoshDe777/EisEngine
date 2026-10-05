#pragma once

#include <vector>
#include "../Vector3.h"

namespace EisEngine::rendering {
    /// {Abstract class} Contains vertex and edge data for meshes.
    struct PrimitiveMesh {
    public:
        /// Creates a new primitive mesh struct.
        /// @param shapeVertices: vector[Vector3] - a list of vertex positions, in object space (relative to object centre).
        /// @param shapeIndices: vector[uint] - a list of indices pointing to individual vertices making up triangles for rendering. MUST be a multiple of three.
        PrimitiveMesh(const std::vector<Vector3>& shapeVertices, const std::vector<unsigned int>& shapeIndices) :
                indices(shapeIndices),
                indexCount(static_cast<int>(shapeIndices.size())) { }

        /// The triangle indices for this mesh. Deliberately immutable after assignment.
        const std::vector<unsigned int> indices;
        /// The amount of indices used. Deliberately immutable after assignment.
        const int indexCount;

        /// Access the mesh's vertices.
        [[nodiscard]] virtual std::vector<Vector3> GetVertices() const = 0;
    };
}
