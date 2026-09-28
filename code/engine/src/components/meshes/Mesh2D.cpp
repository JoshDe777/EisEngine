#include "engine/components/meshes/Mesh2D.h"
#include "engine/utilities/rendering/MeshUtilities.h"
#include "engine/ecs/Entity.h"

namespace EisEngine::components {
    // constructors
    Mesh2D::Mesh2D(Game &engine, guid_t owner, const PrimitiveMesh2D &_primitive):
            Component(engine, owner),
            primitive(_primitive),
            VBO(CreateBuffer(GL_ARRAY_BUFFER, _primitive.vertices)),
            EBO(CreateBuffer(GL_ELEMENT_ARRAY_BUFFER, _primitive.indices)) { }

    Mesh2D::Mesh2D(EisEngine::components::Mesh2D &&other) noexcept  :
            Component(other),
            primitive(other.primitive)
    {
        // copy over data - owner ID, vertex & index buffers.
        owner = other.owner;
        std::swap(this->VBO, other.VBO);
        std::swap(this->EBO, other.EBO);
    }

    // destructor
    void Mesh2D::Invalidate() {
        // formally delete buffers, then call superclass invalidate.
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
        Component::Invalidate();
    }

    // draw
    void Mesh2D::draw() const {
        // bind vertex buffer, enable the VAO pointer & point it to element 0 on the shader.
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), nullptr);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

        glDrawElements(GL_TRIANGLES, primitive.indexCount, GL_UNSIGNED_INT, nullptr);
    }
}