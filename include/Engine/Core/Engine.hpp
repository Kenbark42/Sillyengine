// include/Engine/Core/Engine.hpp
#pragma once

#include <memory>
#include <string>
#include <vector>

#include "Engine/Core/Time.hpp"
#include "Engine/Core/Logger.hpp"
#include "Engine/Core/Config.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include "Engine/Input/InputManager.hpp"
#include "Engine/Physics/PhysicsWorld.hpp"
#include "Engine/Audio/AudioManager.hpp"
#include "Engine/Resources/ResourceManager.hpp"
#include "Engine/Scene/SceneManager.hpp"

namespace Engine
{

    /**
     * @brief Main engine class that coordinates all subsystems
     *
     * The Engine class is the central point of the game engine. It initializes and manages
     * all the subsystems, runs the main game loop, and handles the lifecycle of the application.
     */
    class Engine
    {
    public:
        /**
         * @brief Constructs the engine with the specified configuration
         * @param config Engine configuration
         */
        Engine(const Config &config);

        /**
         * @brief Destructor
         */
        ~Engine();

        /**
         * @brief Initializes all engine subsystems
         * @return True if initialization succeeded, false otherwise
         */
        bool initialize();

        /**
         * @brief Starts the main game loop
         * @return Exit code
         */
        int run();

        /**
         * @brief Shuts down all engine subsystems
         */
        void shutdown();

        /**
         * @brief Gets the renderer subsystem
         * @return Reference to the renderer
         */
        Renderer &getRenderer() { return *renderer; }

        /**
         * @brief Gets the input manager subsystem
         * @return Reference to the input manager
         */
        InputManager &getInputManager() { return *inputManager; }

        /**
         * @brief Gets the physics world subsystem
         * @return Reference to the physics world
         */
        PhysicsWorld &getPhysicsWorld() { return *physicsWorld; }

        /**
         * @brief Gets the audio manager subsystem
         * @return Reference to the audio manager
         */
        AudioManager &getAudioManager() { return *audioManager; }

        /**
         * @brief Gets the resource manager subsystem
         * @return Reference to the resource manager
         */
        ResourceManager &getResourceManager() { return *resourceManager; }

        /**
         * @brief Gets the scene manager subsystem
         * @return Reference to the scene manager
         */
        SceneManager &getSceneManager() { return *sceneManager; }

        /**
         * @brief Gets the time subsystem
         * @return Reference to the time subsystem
         */
        Time &getTime() { return time; }

    private:
        /**
         * @brief Processes a single frame of the game
         */
        void processFrame();

        /**
         * @brief Flag that indicates if the engine is running
         */
        bool running;

        /**
         * @brief Engine configuration
         */
        Config config;

        /**
         * @brief Time manager
         */
        Time time;

        /**
         * @brief Renderer subsystem
         */
        std::unique_ptr<Renderer> renderer;

        /**
         * @brief Input manager subsystem
         */
        std::unique_ptr<InputManager> inputManager;

        /**
         * @brief Physics world subsystem
         */
        std::unique_ptr<PhysicsWorld> physicsWorld;

        /**
         * @brief Audio manager subsystem
         */
        std::unique_ptr<AudioManager> audioManager;

        /**
         * @brief Resource manager subsystem
         */
        std::unique_ptr<ResourceManager> resourceManager;

        /**
         * @brief Scene manager subsystem
         */
        std::unique_ptr<SceneManager> sceneManager;
    };

} // namespace Engine

// src/Engine/Core/Engine.cpp
#include "Engine/Core/Engine.hpp"

namespace Engine
{

    Engine::Engine(const Config &config) : running(false),
                                           config(config),
                                           time()
    {
        Logger::init(config.logLevel);
        Logger::info("Engine created");
    }

    Engine::~Engine()
    {
        if (running)
        {
            shutdown();
        }
        Logger::info("Engine destroyed");
    }

    bool Engine::initialize()
    {
        Logger::info("Initializing engine...");

        // Create subsystems
        resourceManager = std::make_unique<ResourceManager>();
        if (!resourceManager->initialize())
        {
            Logger::error("Failed to initialize resource manager");
            return false;
        }

        renderer = std::make_unique<Renderer>(config.renderer);
        if (!renderer->initialize(config.windowWidth, config.windowHeight, config.windowTitle))
        {
            Logger::error("Failed to initialize renderer");
            return false;
        }

        inputManager = std::make_unique<InputManager>();
        if (!inputManager->initialize(renderer->getWindow()))
        {
            Logger::error("Failed to initialize input manager");
            return false;
        }

        physicsWorld = std::make_unique<PhysicsWorld>(config.physics);
        if (!physicsWorld->initialize())
        {
            Logger::error("Failed to initialize physics world");
            return false;
        }

        audioManager = std::make_unique<AudioManager>();
        if (!audioManager->initialize())
        {
            Logger::error("Failed to initialize audio manager");
            return false;
        }

        sceneManager = std::make_unique<SceneManager>(*this);
        if (!sceneManager->initialize())
        {
            Logger::error("Failed to initialize scene manager");
            return false;
        }

        Logger::info("Engine initialized successfully");
        return true;
    }

    int Engine::run()
    {
        if (!renderer || !inputManager || !physicsWorld || !audioManager || !resourceManager || !sceneManager)
        {
            Logger::error("Cannot run engine: Not all subsystems are initialized");
            return -1;
        }

        running = true;
        Logger::info("Starting main loop");

        // Reset the timer
        time.reset();

        // Main game loop
        while (running)
        {
            processFrame();

            // Check if we should quit
            if (renderer->shouldClose() || inputManager->isKeyPressed(Key::Escape))
            {
                running = false;
            }
        }

        Logger::info("Main loop ended");
        return 0;
    }

    void Engine::processFrame()
    {
        // Update time
        time.update();

        // Process input
        inputManager->update();

        // Update scene (this will update all entities and systems)
        sceneManager->update(time.getDeltaTime());

        // Update physics
        physicsWorld->update(time.getDeltaTime());

        // Render frame
        renderer->beginFrame();
        sceneManager->render();
        renderer->endFrame();

        // Update audio
        audioManager->update();
    }

    void Engine::shutdown()
    {
        Logger::info("Shutting down engine...");

        // Shutdown subsystems in reverse order of initialization
        if (sceneManager)
        {
            sceneManager->shutdown();
            sceneManager.reset();
        }

        if (audioManager)
        {
            audioManager->shutdown();
            audioManager.reset();
        }

        if (physicsWorld)
        {
            physicsWorld->shutdown();
            physicsWorld.reset();
        }

        if (inputManager)
        {
            inputManager->shutdown();
            inputManager.reset();
        }

        if (renderer)
        {
            renderer->shutdown();
            renderer.reset();
        }

        if (resourceManager)
        {
            resourceManager->shutdown();
            resourceManager.reset();
        }

        running = false;
        Logger::info("Engine shut down successfully");
    }

} // namespace Engine