#include "engine/components/rendering/renderers/Renderer.h"

namespace EisEngine::components {

    Renderer::Renderer(Game &engine,
                       guid_t owner,
                       const Color& color,
                       std::string  layer) :
            Component(engine, owner),
            m_color(color),
            m_layer(std::move(layer)) { }

    Renderer::Renderer(EisEngine::components::Renderer &&other) noexcept :
            Component(other) {
        owner = other.owner;
        std::swap(this->m_color, other.m_color);
        std::swap(this->m_layer, other.m_layer);
    }

    // applies the selected color to the active shader.
    void Renderer::ApplyData(Shader& shader) { shader.setVector("color", (glm::vec4) m_color);}
}