#pragma once

#include "PrimitiveMesh.h"

namespace EisEngine {
    namespace components{ class Mesh3D;}
    namespace rendering {

    /// Contains vertex and edge data for 3D meshes.
    struct PrimitiveMesh3D : public PrimitiveMesh {
        friend EisEngine::components::Mesh3D;
    public:
        /// Creates a new primitive 3D mesh.
        /// @param shapeVertices: vector[Vector3] - a list of vertex positions, in object space (relative to object centre).
        /// @param shapeIndices: vector[uint] - a list of indices pointing to individual vertices making up triangles for rendering. MUST be a multiple of three.
        /// @param shapeNormals: vector[Vector3]* - a list of normals for every vertex in the mesh, in object space. If null, generates a default set.
        /// @param shapeUVs: vector[Vector2]* - a list of coordinates in texture space (2D) associated with every vertex for texture mapping. If null, generates a default set.
        /// @param shapeTangents: vector[Vector3]* - a list of tangents for every vertex in the mesh, in object space. If null, generates a default set based on normals.
        /// @param shapeBitangents: vector[Vector3]* - a list of bitangents for every vertex in the mesh, in object space. If null, generates a default set based on normals and tangents.
        explicit PrimitiveMesh3D(const std::vector<Vector3> &shapeVertices,
                                 const std::vector<unsigned int> &shapeIndices,
                                 const std::vector<Vector3>* shapeNormals = nullptr,
                                 const std::vector<Vector2>* shapeUVs = nullptr,
                                 const std::vector<Vector3>* shapeTangents = nullptr,
                                 const std::vector<Vector3>* shapeBitangents = nullptr);

        /// A skybox cube primitive.
        static const PrimitiveMesh3D skybox;
        /// A regular cube primitive.
        static const PrimitiveMesh3D cube;

        /// Access a few counts
        [[nodiscard]] unsigned int GetVertexCount() const { return nVerts;}

        /// Access the mesh's vertices.
        [[nodiscard]] std::vector<Vector3> GetVertices() const override;
        /// Access the mesh's normals.
        [[nodiscard]] std::vector<Vector3> GetNormals() const;
        /// Access the mesh's UVs.
        [[nodiscard]] std::vector<Vector2> GetUVs() const;
        /// Access the mesh's tangents.
        [[nodiscard]] std::vector<Vector3> GetTangents() const;
        /// Access the mesh's bitangents.
        [[nodiscard]] std::vector<Vector3> GetBitangents() const;
    private:
        /// The vertices forming the mesh.
        const std::vector<glm::vec3> vertices;
        /// The vectors describing light interaction on a given vertex.
        const std::vector<glm::vec3> normals;
        /// The vertex mapping to texture coordinates.
        const std::vector<glm::vec2> uvs;
        /// The vectors describing light reflection in world space in glassy shader
        std::vector<glm::vec3> tangents;
        /// The vectors describing light reflection in world space in glassy shader
        std::vector<glm::vec3> bitangents;
        /// The amount of vertices in this mesh.
        const unsigned int nVerts;
        /// Calculates a set of tangent and bitangent vectors based on normals.
        void CalculateTangentVecs();
    };
    } 
} 
