#include "engine/components/Renderer.h"
#include "engine/components/PointLight.h"
#include "engine/ResourceManager.h"
#include "engine/utilities/rendering/Shader.h"
#include "engine/components/meshes/Mesh.h"

namespace EisEngine::components {
    Renderer::Renderer(EisEngine::Game &engine,
                       EisEngine::ecs::guid_t owner, 
                       EisEngine::rendering::ShaderType shader,
                       EisEngine::Texture2D* diffTex,
                       EisEngine::Material* mat,
                       std::string layer,
                       Texture2D* normMap) :
            shader(shader),
            Component(engine, owner),
            diffuseTexture(diffTex),
            material(mat),
            m_layer(std::move(layer)),
            normalMap(normMap) {
        // assigns default blank textures & materials if none are passed in.
        if(!diffTex)
            diffuseTexture = ResourceManager::GetTexture("default");
        if(!mat)
            material = ResourceManager::CreateMaterialInstance("default");
        if(!normMap)
            normalMap = ResourceManager::GetTexture("default_normal");

        ResourceManager::GetShader(EisEngine::rendering::to_string(shader))->_register(this);
    }

    Renderer::Renderer(EisEngine::components::Renderer &&other) noexcept :
            Component(other),
            shader(std::move(other.shader)),
            diffuseTexture(std::move(other.diffuseTexture)),
            normalMap(std::move(other.normalMap)),
            material(std::move(other.material)),
            m_layer(std::move(other.m_layer)){
        owner = other.owner;
        other.owner = -1;
    }

    // applies the selected color to the active shader.
    void Renderer::ApplyData(Shader& shader) {
        material->ApplyMatData(shader);

        if(diffuseTexture != nullptr){
            auto diffPos = glGetUniformLocation(shader.GetShaderID(), "image");
            if(diffPos != -1) {
                shader.ApplyTexture2D(*diffuseTexture, UniformSamplerIndices::DIFFUSE);
            }
        }


        if(normalMap != nullptr) {
            auto nPos = glGetUniformLocation(shader.GetShaderID(), "nMap");
            if(nPos != -1) {
                shader.ApplyTexture2D(*normalMap, UniformSamplerIndices::NORMAL);
                DEBUG_INFO(entity()->name() + " normal map applied.")
            }
        }
    }

    void Renderer::ApplyTextureOfType(Shader* shader, const UniformSamplerIndices& type, const std::string& uniformLocation) {
        // Cubemaps not supported with base Renderer (see CubemapRenderer), & depth maps stored differently as well.
        std::unordered_map<UniformSamplerIndices, Texture2D*> textures = {
            {UniformSamplerIndices::DIFFUSE, diffuseTexture},
            {UniformSamplerIndices::NORMAL, normalMap}
        };

        // try getting the sought after map.
        Texture2D* sampler;
        try {
            sampler = textures.at(type);
        }
        catch (const std::exception& e) {
            // if the texture is unsupported, return.
            DEBUG_ERROR("[Renderer::ApplyTextureOfType( '" + to_string(type) + "' )] ERROR: " + e.what())
            return;
        }

        if (sampler == nullptr)
            // maybe here get/assign a default texture then continue. Returning for now.
            return;

        GLint location = glGetUniformLocation(shader->GetShaderID(), uniformLocation.c_str());
        if (location != -1)
            shader->ApplyTexture2D(*sampler, type);
        else
            DEBUG_WARN("Could not find texture '" + uniformLocation + "' in Shader Program!")
    }

    Mesh* Renderer::GetMesh() const {
        return entity()->GetComponent<Mesh>();
    }

    void Renderer::Invalidate() {
        ResourceManager::GetShader(EisEngine::rendering::to_string(shader))->_deregister(this);
    }
}