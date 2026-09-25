#include "engine/systems/RenderingSystem.h"
#include "engine/Game.h"
#include "engine/Components.h"

namespace EisEngine::systems {
// helper functions:

    // used to sort entities by ascending z position.
    bool CompareZValues(SpriteMesh* a, SpriteMesh* b)
    { return a->entity()->transform->GetGlobalPosition().z < b->entity()->transform->GetGlobalPosition().z;}

// rendering system methods:

    RenderingSystem::RenderingSystem(EisEngine::Game &engine) : System(engine) {
        camera = &engine.camera;
        if(!camera)
            DEBUG_RUNTIME_ERROR("Cannot initialize rendering; Camera not found.")

        engine.onUpdate.addListener([&] (Game& engine){ Draw();});

        VAO = {};
        for(unsigned int & i : VAO)
            glGenVertexArrays(1, &i);

        // generate default shader (mesh2D & lines)
        ResourceManager::GenerateShaderFromFiles("shaders/vertexShader.vert",
                                                 "shaders/fragmentShader.frag",
                                                 "Default Shader");
        // generate default sprite shader
        ResourceManager::GenerateShaderFromFiles( "shaders/spriteVertexShader.vert",
                                                  "shaders/spriteFragmentShader.frag",
                                                  "Sprite Shader");
        // generate ui sprite shader
        ResourceManager::GenerateShaderFromFiles("shaders/uiVertexShader.vert",
                                                 "shaders/spriteFragmentShader.frag",
                                                 "UI Shader");
    }

    void RenderingSystem::Draw() {
        // re-enable depth testing for 'regular' entities.
        glEnable(GL_DEPTH_TEST);
        auto vpMatrix = camera->GetVPMatrix();

        auto activeShader = ResourceManager::GetShader("Default Shader");

        // Mesh2D rendering
        if(engine.componentManager.hasComponentOfType<Mesh2D>()){
            glBindVertexArray(VAO[0]);
            activeShader->Apply(vpMatrix);
            engine.componentManager.forEachComponent<Mesh2D>([&](Mesh2D& mesh){
                auto model = mesh.entity()->transform->GetModelMatrix();
                activeShader->setMatrix("mvp", activeShader->CalculateMVPMatrix(model));
                auto renderer = mesh.entity()->GetComponent<Mesh2DRenderer>();
                if(renderer)
                    renderer->ApplyData(*activeShader);
                mesh.draw();
            });
        }

        // line rendering (same shader as Mesh2D's)
        if(engine.componentManager.hasComponentOfType<Line>()){
            glBindVertexArray(VAO[1]);
            engine.componentManager.forEachComponent<Line>([&] (Line& mesh){
                auto renderer = mesh.entity()->GetComponent<LineRenderer>();
                if(renderer)
                    renderer->ApplyData(*activeShader);
                auto model = mesh.entity()->transform->GetModelMatrix();
                activeShader->setMatrix("mvp", activeShader->CalculateMVPMatrix(model));
                mesh.draw();
            });
        }

        activeShader = ResourceManager::GetShader("Sprite Shader");

        // Sprite rendering

        // weed out UI Sprites for later overlay rendering
        std::vector<SpriteMesh*> uiSprites = {};

        if(engine.componentManager.hasComponentOfType<SpriteMesh>()){
            glBindVertexArray(VAO[2]);
            activeShader->Apply(vpMatrix);
            engine.componentManager.forEachComponent<SpriteMesh>([&] (SpriteMesh& mesh){
                auto renderer = mesh.entity()->GetComponent<SpriteRenderer>();
                if(!renderer){
                    DEBUG_ERROR("No sprite renderer attached to mesh on entity " + mesh.entity()->name())
                    return;
                }
                if(renderer->GetLayer() == "UI"){
                    uiSprites.emplace_back(&mesh);
                    return;
                }
                renderer->ApplyData(*activeShader);
                auto model = mesh.entity()->transform->GetModelMatrix();
                activeShader->setMatrix("mvp", activeShader->CalculateMVPMatrix(model));
                mesh.draw();
            });
        }

        // return if no UI sprites to render
        if(uiSprites.empty())
            return;

        // disable depth testing here for UI
        glDisable(GL_DEPTH_TEST);

        // sort by ascending z values for layering because no depth test.
        std::sort(uiSprites.begin(), uiSprites.end(), CompareZValues);

        activeShader = ResourceManager::GetShader("UI Shader");

        glBindVertexArray(VAO[3]);
        activeShader->Apply(vpMatrix);
        for (auto mesh : uiSprites) {
            auto renderer = mesh->entity()->GetComponent<SpriteRenderer>();
            renderer->ApplyData(*activeShader);
            auto screenWidth = camera->GetWidth();
            auto screenHeight = camera->GetHeight();
            auto projection = glm::ortho(-(float) screenWidth / 2, (float) screenWidth / 2,
                                         - (float) screenHeight / 2, (float) screenHeight / 2);
            auto modelProjection = projection * mesh->entity()->transform->GetModelMatrix();
            activeShader->setMatrix("mvp", modelProjection);
            mesh->draw();
        }
    }
}
