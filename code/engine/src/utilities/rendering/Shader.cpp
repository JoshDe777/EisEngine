#include "glm/gtc/type_ptr.hpp"
#include "engine/utilities/rendering/Shader.h"
#include "engine/ResourceManager.h"
#include "engine/utilities/Color.h"

namespace EisEngine::rendering {
    Shader::Shader(const unsigned int &vertexShaderProgram, const unsigned int &fragmentShaderProgram) :
    vertexShader(vertexShaderProgram),
    fragmentShader(fragmentShaderProgram) {
        shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);
        glDetachShader(shaderProgram, vertexShader);
        glDetachShader(shaderProgram, fragmentShader);
    }

    Shader::Shader(EisEngine::rendering::Shader &&other) noexcept {
        std::swap(this->shaderProgram, other.shaderProgram);
        std::swap(this->vertexShader, other.vertexShader);
        std::swap(this->fragmentShader, other.fragmentShader);
    }

    void Shader::Invalidate() const {
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
    }

    void Shader::Apply(const glm::mat4& newVPMatrix) {
        glUseProgram(shaderProgram);
        vpMatrix = newVPMatrix;
        setMatrix("mvp", vpMatrix);
        setVector("color", (glm::vec4) Color::white);
    }

    void Shader::ApplyTexture(const Texture2D& texture) {
        glActiveTexture(GL_TEXTURE0);
        texture.Bind();
    }

    void Shader::setMatrix(const std::string &uniformName, glm::mat4 matrix) const {
        auto uniformLocation = glGetUniformLocation(shaderProgram, uniformName.c_str());
        glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(matrix));
    }
    void Shader::setVector(const std::string &uniformName, glm::vec4 vector) const {
        auto uniformLocation = glGetUniformLocation(shaderProgram, uniformName.c_str());
        glUniform4fv(uniformLocation, 1, glm::value_ptr(vector));
    }

    const fs::path Shader::defaultVertexShaderPath = "shaders/vertexShader.vert";
    const fs::path Shader::defaultFragmentShaderPath = "shaders/fragmentShader.frag";
    const fs::path Shader::spriteVertexShaderPath = "shaders/spriteVertexShader.vert";
    const fs::path Shader::spriteFragmentShaderPath = "shaders/spriteFragmentShader.frag";
    const fs::path Shader::uiVertexShaderPath = "shaders/uiVertexShader.vert";
}
