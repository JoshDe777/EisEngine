#include "glm/gtc/type_ptr.hpp"
#include "engine/utilities/rendering/Shader.h"
#include "engine/ResourceManager.h"
#include "engine/utilities/Color.h"
#include "engine/systems/Camera.h"
#include "engine/components/Renderer.h"

namespace EisEngine::rendering {
    Shader::Shader(
            const unsigned int &vertexShaderProgram,
            const unsigned int &fragmentShaderProgram,
            const std::string& name
        ) :
    vertexShader(vertexShaderProgram),
    fragmentShader(fragmentShaderProgram),
    name(name){
        shaderProgram = glCreateProgram();

        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);

        glLinkProgram(shaderProgram);

        glValidateProgram(shaderProgram);

        DEBUG_OPENGL("Shader " + name)

        GLint status;
        glGetProgramiv(shaderProgram, GL_VALIDATE_STATUS, &status);
        if (status != GL_TRUE)
        {
            char log[1024];
            glGetProgramInfoLog(shaderProgram, 1024, nullptr, log);
            std::cerr << name << " - " << log << std::endl;
        }

        glDetachShader(shaderProgram, vertexShader);
        glDetachShader(shaderProgram, fragmentShader);
    }

    Shader::Shader(EisEngine::rendering::Shader &&other) noexcept {
        std::swap(this->shaderProgram, other.shaderProgram);
        std::swap(this->vertexShader, other.vertexShader);
        std::swap(this->fragmentShader, other.fragmentShader);
    }

    void Shader::Invalidate() {
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        // release the assigned renderers pointers.
        assignedRenderers.clear();
    }

    void Shader::Apply(Camera* camera) {
        glUseProgram(shaderProgram);
        DEBUG_OPENGL("Shader " + name)
        vpMatrix = camera->GetVPMatrix();
        GLint mvpLoc = glGetUniformLocation(shaderProgram, "mvp");
        if(mvpLoc != -1)
            setMatrix("mvp", vpMatrix);
        DEBUG_OPENGL("Shader " + name)
            GLint camPosLoc = glGetUniformLocation(shaderProgram, "camPos");
        if(camPosLoc != -1)
            setVector("camPos", camera->transform->GetGlobalPosition());
        DEBUG_OPENGL("Shader " + name)

        setInt("image", UniformSamplerIndices::DIFFUSE);
        setInt("nMap", UniformSamplerIndices::NORMAL);
        setInt("cubeMap", UniformSamplerIndices::CUBEMAP);
        setInt("backDepthMap", UniformSamplerIndices::DEPTH_BACK_FACE);
        setInt("frontDepthMap", UniformSamplerIndices::DEPTH_FRONT_FACE);
        DEBUG_OPENGL("Shader " + name)
    }

    void Shader::ApplyTexture2D(const Texture2D& texture, UniformSamplerIndices type) const {
        GLint maxUnits = 0;
        glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &maxUnits);
        if(type > maxUnits){
            DEBUG_WARN("Attempting to bind texture to invalid ID!")
            return;
        }

        glActiveTexture(GL_TEXTURE0 + type);
        texture.Bind();
        DEBUG_OPENGL("Shader " + name)
    }

    void Shader::ApplyCubemap(const Cubemap& cubemap) const {
        glActiveTexture(GL_TEXTURE0 + UniformSamplerIndices::CUBEMAP);
        cubemap.Bind();
        DEBUG_OPENGL("Shader " + name)
    }

    void Shader::setMatrix(const std::string &uniformName, glm::mat4 mat4) const {
        GLint uniformLocation = glGetUniformLocation(shaderProgram, uniformName.c_str());
        if(uniformLocation != -1)
            glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(mat4));
    }
    void Shader::setMatrix(const std::string &uniformName, glm::mat3 mat3) const {
        GLint uniformLocation = glGetUniformLocation(shaderProgram, uniformName.c_str());
        if(uniformLocation != -1)
            glUniformMatrix3fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(mat3));
    }
    void Shader::setVector(const std::string &uniformName, glm::vec4 vec4) const {
        GLint uniformLocation = glGetUniformLocation(shaderProgram, uniformName.c_str());
        if(uniformLocation != -1)
            glUniform4fv(uniformLocation, 1, glm::value_ptr(vec4));
    }
    void Shader::setVector(const std::string &uniformName, glm::vec3 vec3) const {
        GLint uniformLocation = glGetUniformLocation(shaderProgram, uniformName.c_str());
        if(uniformLocation != -1)
            glUniform3fv(uniformLocation, 1, glm::value_ptr(vec3));
    }

    void Shader::setInt(const std::string &uniformName, const int &val) const {
        GLint uniformLocation = glGetUniformLocation(shaderProgram, uniformName.c_str());
        if(uniformLocation != -1)
            glUniform1i(uniformLocation, val);
    }

    void Shader::setFloat(const std::string &uniformName, const float &val) const {
        GLint uniformLocation = glGetUniformLocation(shaderProgram, uniformName.c_str());
        if(uniformLocation != -1)
            glUniform1f(uniformLocation, val);
    }

    void Shader::_register(Renderer* renderer) {
        assignedRenderers.push_back(renderer);
        nRenderers++;
    }

    void Shader::_deregister(Renderer* renderer) {
        //nothing to remove if no renderers assigned
        if (nRenderers == 0)
            return;

        std::vector<Renderer*>::iterator& it = std::find(assignedRenderers.begin(), assignedRenderers.end(), renderer);
        if (it != assignedRenderers.end()) {
            assignedRenderers.erase(it);
            nRenderers--;
        }
    }
}
