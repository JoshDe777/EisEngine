#pragma once

#include <memory>
#include <stdexcept>
#include <string>
#include <vector>
#include <box2d/box2d.h>
#include <soloud.h>
#include <soloud_wav.h>
#include <OpenGL/OpenGlInclude.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include "engine/Utilities.h"
#include "engine/Systems.h"
#include "engine/Context.h"
#include "engine/ecs/EntityManager.h"

namespace EisEngine {
    using event_t = events::Event<Game, Game&>;
    using ecs::ComponentManager;
    using ecs::EntityManager;
    using components::Transform;
    using ctx::Context;

    /// The central class for games created using Eis-Engine.
    /// Runs the game loop and holds references to all systems required for running a game.
    class Game {
    public:
        /// Creates an instance of a game.
        /// @param title - game window title.
        Game(const std::string &title);
        /// Terminates the instance of the game.
        virtual ~Game();

        /// Runs the game life until demanded to terminate.
        virtual void run();
        /// Signals the engine to close the game window.
        void Quit();

        /// Fetches the game window.
        /// @return @a GLFWwindow* - a pointer to a GLFW window.
        [[nodiscard]] GLFWwindow *getWindow();

        /// an event invoked right at the start of the game's lifetime.
        event_t onStartup;
        /// an event invoked right before the first iteration of the game loop.
        event_t onAfterStartup;
        /// an event invoked at the beginning of every frame.
        event_t onBeforeUpdate;
        /// an event invoked in the middle of every frame.
        event_t onUpdate;
        /// an event invoked at the end of every frame.
        event_t onAfterUpdate;
        /// an event invoked right after the game has been asked to terminate.
        event_t onBeforeShutdown;
        /// an event invoked right before fully shutting down the game.
        event_t onShutdown;
        /// an event invoked every frame when a behaviour is instantiated.
        event_t onEntityStart;

    private:
        /// The EisEngine time manager.
        Time time;

    public:
        /// the game's component manager.
        unique_ptr<ComponentManager> componentManager;
        /// the game's entity manager.
        unique_ptr<EntityManager> entityManager;
        /// The game context. Gives information about the window / software side of the game.
        unique_ptr<Context> context;
        /// the main camera rendering the scene.
        unique_ptr<Camera> camera;
        /// The system running game physics.
        unique_ptr<PhysicsSystem> physics;
        /// The system synchronizing transforms and rigidbodies.
        unique_ptr<PhysicsUpdater> physicsUpdater;
        /// A system used to update light positions
        unique_ptr<LightSystem> lightSystem;
    protected:
        /// Utility function called every frame.
        virtual void update(GLFWwindow *window);
        /// Utility function called once at the start of the game's lifetime.
        virtual void start() {}
        /// Called every frame. Defines conditions for game termination.
        void CheckForCloseWindowSignal();
        /// The game loop, defines the sequence of actions.
        void GameLoop();

        /// A system used to draw lines.
        unique_ptr<RenderingSystem> renderingSystem;
        /// A system tasked with managing transform relations.
        unique_ptr<SceneGraphPruner> sceneGraphPruner;
        /// A system tasked with updating transforms.
        unique_ptr<SceneGraphUpdater> sceneGraphUpdater;
    private:
        /// The EisEngine input manager.
        unique_ptr<Input> input;
    };
}
