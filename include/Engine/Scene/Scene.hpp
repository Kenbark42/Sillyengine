#pragma once

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>

#include "Engine/ECS/Entity.hpp"
#include "Engine/ECS/System.hpp"

namespace Engine
{

    class EntityManager;
    class Engine;

    /**
     * @brief Scene class
     *
     * A scene is a container for entities and systems. It represents a level or area
     * in the game world.
     */
    class Scene
    {
    public:
        /**
         * @brief Constructor
         * @param name Scene name
         * @param engine Reference to the engine
         */
        Scene(const std::string &name, Engine &engine);

        /**
         * @brief Destructor
         */
        ~Scene();

        /**
         * @brief Gets the scene name
         * @return Scene name
         */
        const std::string &getName() const { return name; }

        /**
         * @brief Updates the scene
         * @param deltaTime Time since the last update
         */
        void update(float deltaTime);

        /**
         * @brief Renders the scene
         */
        void render();

        /**
         * @brief Creates an entity
         * @return Pointer to the created entity
         */
        Entity *createEntity();

        /**
         * @brief Creates an entity with a name
         * @param name Entity name
         * @return Pointer to the created entity
         */
        Entity *createEntity(const std::string &name);

        /**
         * @brief Destroys an entity
         * @param entity Pointer to the entity to destroy
         */
        void destroyEntity(Entity *entity);

        /**
         * @brief Gets an entity by ID
         * @param id Entity ID
         * @return Pointer to the entity, or nullptr if not found
         */
        Entity *getEntity(uint32_t id);

        /**
         * @brief Gets an entity by name
         * @param name Entity name
         * @return Pointer to the entity, or nullptr if not found
         */
        Entity *getEntityByName(const std::string &name);

        /**
         * @brief Adds a system to the scene
         * @tparam T System type
         * @tparam Args System constructor argument types
         * @param args System constructor arguments
         * @return Reference to the added system
         */
        template <typename T, typename... Args>
        T &addSystem(Args &&...args)
        {
            static_assert(std::is_base_of<System, T>::value, "T must derive from System");

            // Create system
            auto system = std::make_unique<T>(engine, std::forward<Args>(args)...);
            T &systemRef = *system;

            // Initialize system
            if (!system->initialize())
            {
                throw std::runtime_error("Failed to initialize system");
            }

            // Add system to the map
            systems[typeid(T)] = std::move(system);

            return systemRef;
        }

        /**
         * @brief Gets a system by type
         * @tparam T System type
         * @return Pointer to the system, or nullptr if not found
         */
        template <typename T>
        T *getSystem()
        {
            static_assert(std::is_base_of<System, T>::value, "T must derive from System");

            auto it = systems.find(typeid(T));
            if (it == systems.end())
            {
                return nullptr;
            }

            return static_cast<T *>(it->second.get());
        }

    private:
        /**
         * @brief Scene name
         */
        std::string name;

        /**
         * @brief Reference to the engine
         */
        Engine &engine;

        /**
         * @brief Entity manager
         */
        std::unique_ptr<EntityManager> entityManager;

        /**
         * @brief Map of systems
         */
        std::unordered_map<std::type_index, std::unique_ptr<System>> systems;

        /**
         * @brief Map of entity names
         */
        std::unordered_map<std::string, Entity *> entityNames;
    };

} // namespace Engine