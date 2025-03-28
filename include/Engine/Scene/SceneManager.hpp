#pragma once

#include <string>
#include <unordered_map>
#include <memory>

namespace Engine
{

    class Scene;
    class Engine;

    /**
     * @brief Scene manager class
     *
     * The scene manager is responsible for loading, unloading, and switching between scenes.
     */
    class SceneManager
    {
    public:
        /**
         * @brief Constructor
         * @param engine Reference to the engine
         */
        explicit SceneManager(Engine &engine);

        /**
         * @brief Destructor
         */
        ~SceneManager();

        /**
         * @brief Initializes the scene manager
         * @return True if initialization succeeded, false otherwise
         */
        bool initialize();

        /**
         * @brief Updates the scene manager
         * @param deltaTime Time since the last update
         */
        void update(float deltaTime);

        /**
         * @brief Renders the active scene
         */
        void render();

        /**
         * @brief Shuts down the scene manager
         */
        void shutdown();

        /**
         * @brief Creates a scene
         * @param name Scene name
         * @return Pointer to the created scene
         */
        Scene *createScene(const std::string &name);

        /**
         * @brief Gets a scene by name
         * @param name Scene name
         * @return Pointer to the scene, or nullptr if not found
         */
        Scene *getScene(const std::string &name);

        /**
         * @brief Destroys a scene
         * @param name Scene name
         * @return True if the scene was destroyed, false if it doesn't exist
         */
        bool destroyScene(const std::string &name);

        /**
         * @brief Sets the active scene
         * @param scene Pointer to the scene
         */
        void setActiveScene(Scene *scene);

        /**
         * @brief Gets the active scene
         * @return Pointer to the active scene
         */
        Scene *getActiveScene() const { return activeScene; }

    private:
        /**
         * @brief Reference to the engine
         */
        Engine &engine;

        /**
         * @brief Map of scenes
         */
        std::unordered_map<std::string, std::unique_ptr<Scene>> scenes;

        /**
         * @brief Active scene
         */
        Scene *activeScene;
    };

} // namespace Engine
