#pragma once

#include "PrimitiveMesh.h"
#include "SpriteVertex.h"
#include "../Bounds2D.h"

// this should be replaced by the Primitive 2D mesh with support for UVs...
namespace EisEngine{
    namespace components { class SpriteMesh;}

    namespace rendering {
        /// [DEPRECATING] a primitive sprite mesh. Stores vertex data and maps them to texture coordinates.
        class PrimitiveSpriteMesh : public PrimitiveMesh {
            friend EisEngine::components::SpriteMesh;
        public:
            /// Creates a new primitive sprite mesh from the given information.
            /// @param primitiveVertices - std::vector&lt;Vector3>: A list of vertex coordinates forming the shape.
            /// @param shapeIndices - std::vector&lt;unsigned int>: A list of integers linking vertices to form triangles.
            explicit PrimitiveSpriteMesh(const std::vector<Vector3>& primitiveVertices,
                                         const std::vector<unsigned int>& shapeIndices);

            /// Access the mesh's vertices.
            [[nodiscard]] std::vector<Vector3> GetVertices() const override;

            /// A simple 1x1 square mesh to hold a Sprite.
            static const PrimitiveSpriteMesh SquareSpriteMesh;

            Bounds2D bounds;
        private:
            /// A list of Sprite Vertices for the current mesh.
            /// Each SpriteVertex contains:
            /// @b glm::vec3: Vertex coordinate in model space (relative to object center).
            /// @b glm::vec2: Sprite coordinate, mapping the vertex to a coordinate within texture space.
            const std::vector<SpriteVertex> vertices;
        };
    }
}
