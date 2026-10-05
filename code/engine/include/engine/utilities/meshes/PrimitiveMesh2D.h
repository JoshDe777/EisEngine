#pragma once

#include "PrimitiveMesh.h"

namespace EisEngine{
    namespace components{ class Mesh2D;}
    namespace rendering {
        /// Contains vertex and edge data for 2D meshes.
    struct PrimitiveMesh2D : public PrimitiveMesh {
        friend EisEngine::components::Mesh2D;
    public:
        /// Creates a new primitive 2D mesh.
        /// @param shapeVertices: vector[Vector3] - a list of vertex positions, in object space (relative to object centre).
        /// @param shapeIndices: vector[uint] - a list of indices pointing to individual vertices making up triangles for rendering. MUST be a multiple of three.
        explicit PrimitiveMesh2D(const std::vector<Vector3> &primitiveVertices,
                                 const std::vector<unsigned int> &shapeIndices);
        /// A standard 1x1 square.
        static const PrimitiveMesh2D Square;
        /// Access the mesh's vertices.
        [[nodiscard]] std::vector<Vector3> GetVertices() const override;
    private:
        /// The vertices forming the mesh.
        const std::vector<glm::vec3> vertices;
    };
}
}
