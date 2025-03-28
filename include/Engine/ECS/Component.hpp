// include/Engine/ECS/Component.hpp
#pragma once

#include <typeindex>
#include <typeinfo>

namespace Engine
{

    class Entity;

    /**
     * @brief Base class for all components
     *
     * Components are data containers that can be attached to entities.
     * They don't contain any logic, just data.
     */
    class Component
    {
    public:
        /**
         * @brief Default constructor
         */
        Component() : owner(nullptr) {}

        /**
         * @brief Virtual destructor
         */
        virtual ~Component() = default;

        /**
         * @brief Gets the component type
         * @return Type index of the component
         */
        virtual std::type_index getType() const = 0;

        /**
         * @brief Gets the owner entity
         * @return Pointer to the owner entity
         */
        Entity *getOwner() const { return owner; }

        /**
         * @brief Sets the owner entity
         * @param entity Pointer to the owner entity
         */
        void setOwner(Entity *entity) { owner = entity; }

    private:
        /**
         * @brief Pointer to the owner entity
         */
        Entity *owner;
    };

    /**
     * @brief Templated component class
     * @tparam T Component type
     */
    template <typename T>
    class ComponentT : public Component
    {
    public:
        /**
         * @brief Gets the component type
         * @return Type index of the component
         */
        std::type_index getType() const override
        {
            return std::type_index(typeid(T));
        }

        /**
         * @brief Static method to get the component type
         * @return Type index of the component
         */
        static std::type_index staticType()
        {
            return std::type_index(typeid(T));
        }
    };

} // namespace Engine

// include/Engine/ECS/Entity.hpp
#pragma once

#include <memory>
#include <unordered_map>
#include <typeindex>
#include <string>

#include "Engine/ECS/Component.hpp"
#include "Engine/Math/Transform.hpp"

namespace Engine
{

    class EntityManager;

    /**
     * @brief Entity class representing a game object
     *
     * Entities are containers for components. They don't contain any logic,
     * just an ID, a name, and a transform.
     */
    class Entity
    {
    public:
        /**
         * @brief Default constructor
         */
        Entity(EntityManager *manager, uint32_t id);

        /**
         * @brief Destructor
         */
        ~Entity();

        /**
         * @brief Gets the entity ID
         * @return Entity ID
         */
        uint32_t getId() const { return id; }

        /**
         * @brief Gets the entity name
         * @return Entity name
         */
        const std::string &getName() const { return name; }

        /**
         * @brief Sets the entity name
         * @param name Entity name
         */
        void setName(const std::string &name) { this->name = name; }

        /**
         * @brief Gets the entity transform
         * @return Reference to the transform
         */
        Transform &getTransform() { return transform; }

        /**
         * @brief Gets the entity transform (const)
         * @return Const reference to the transform
         */
        const Transform &getTransform() const { return transform; }

        /**
         * @brief Checks if the entity is active
         * @return True if the entity is active, false otherwise
         */
        bool isActive() const { return active; }

        /**
         * @brief Sets the entity active state
         * @param active True to activate the entity, false to deactivate
         */
        void setActive(bool active) { this->active = active; }

        /**
         * @brief Adds a component to the entity
         * @tparam T Component type
         * @tparam Args Component constructor argument types
         * @param args Component constructor arguments
         * @return Reference to the added component
         */
        template <typename T, typename... Args>
        T &addComponent(Args &&...args)
        {
            static_assert(std::is_base_of<Component, T>::value, "T must derive from Component");

            // Check if component already exists
            auto type = std::type_index(typeid(T));
            if (components.find(type) != components.end())
            {
                throw std::runtime_error("Component already exists");
            }

            // Create component
            T *component = new T(std::forward<Args>(args)...);
            component->setOwner(this);

            // Store component
            components[type] = std::unique_ptr<Component>(component);

            return *component;
        }

        /**
         * @brief Gets a component from the entity
         * @tparam T Component type
         * @return Reference to the component
         * @throws std::runtime_error if the component doesn't exist
         */
        template <typename T>
        T &getComponent()
        {
            static_assert(std::is_base_of<Component, T>::value, "T must derive from Component");

            // Check if component exists
            auto type = std::type_index(typeid(T));
            auto it = components.find(type);
            if (it == components.end())
            {
                throw std::runtime_error("Component does not exist");
            }

            return *static_cast<T *>(it->second.get());
        }

        /**
         * @brief Checks if the entity has a component
         * @tparam T Component type
         * @return True if the entity has the component, false otherwise
         */
        template <typename T>
        bool hasComponent() const
        {
            static_assert(std::is_base_of<Component, T>::value, "T must derive from Component");

            auto type = std::type_index(typeid(T));
            return components.find(type) != components.end();
        }

        /**
         * @brief Removes a component from the entity
         * @tparam T Component type
         * @return True if the component was removed, false if it didn't exist
         */
        template <typename T>
        bool removeComponent()
        {
            static_assert(std::is_base_of<Component, T>::value, "T must derive from Component");

            auto type = std::type_index(typeid(T));
            auto it = components.find(type);
            if (it == components.end())
            {
                return false;
            }

            components.erase(it);
            return true;
        }

    private:
        /**
         * @brief Entity manager
         */
        EntityManager *manager;

        /**
         * @brief Entity ID
         */
        uint32_t id;

        /**
         * @brief Entity name
         */
        std::string name;

        /**
         * @brief Entity transform
         */
        Transform transform;

        /**
         * @brief Entity active state
         */
        bool active;

        /**
         * @brief Map of components
         */
        std::unordered_map<std::type_index, std::unique_ptr<Component>> components;
    };

} // namespace Engine

// include/Engine/ECS/System.hpp
#pragma once

#include <vector>
#include <string>
#include <unordered_set>
#include <typeindex>

namespace Engine
{

    class Entity;
    class Engine;

    /**
     * @brief Base class for all systems
     *
     * Systems contain the logic to process entities with specific components.
     */
    class System
    {
    public:
        /**
         * @brief Constructor
         * @param engine Reference to the engine
         */
        explicit System(Engine &engine);

        /**
         * @brief Virtual destructor
         */
        virtual ~System() = default;

        /**
         * @brief Initializes the system
         * @return True if initialization succeeded, false otherwise
         */
        virtual bool initialize() = 0;

        /**
         * @brief Updates the system
         * @param deltaTime Time since the last update
         */
        virtual void update(float deltaTime) = 0;

        /**
         * @brief Shuts down the system
         */
        virtual void shutdown() = 0;

        /**
         * @brief Adds an entity to the system
         * @param entity Pointer to the entity
         */
        void addEntity(Entity *entity);

        /**
         * @brief Removes an entity from the system
         * @param entity Pointer to the entity
         */
        void removeEntity(Entity *entity);

        /**
         * @brief Gets the system name
         * @return System name
         */
        const std::string &getName() const { return name; }

    protected:
        /**
         * @brief Reference to the engine
         */
        Engine &engine;

        /**
         * @brief System name
         */
        std::string name;

        /**
         * @brief List of entities processed by the system
         */
        std::vector<Entity *> entities;

        /**
         * @brief Set of component types required by the system
         */
        std::unordered_set<std::type_index> requiredComponents;

        /**
         * @brief Checks if an entity has all required components
         * @param entity Pointer to the entity
         * @return True if the entity has all required components, false otherwise
         */
        bool hasRequiredComponents(Entity *entity) const;
    };

} // namespace Engine

// include/Engine/ECS/EntityManager.hpp
#pragma once

#include <memory>
#include <unordered_map>
#include <vector>
#include <queue>

#include "Engine/ECS/Entity.hpp"
#include "Engine/ECS/System.hpp"

namespace Engine
{

    /**
     * @brief Manager for entities and systems
     *
     * The entity manager creates and destroys entities, and keeps track of
     * all systems.
     */
    class EntityManager
    {
    public:
        /**
         * @brief Constructor
         * @param engine Reference to the engine
         */
        explicit EntityManager(Engine &engine);

        /**
         * @brief Destructor
         */
        ~EntityManager();

        /**
         * @brief Initializes the entity manager
         * @return True if initialization succeeded, false otherwise
         */
        bool initialize();

        /**
         * @brief Updates all systems
         * @param deltaTime Time since the last update
         */
        void update(float deltaTime);

        /**
         * @brief Shuts down the entity manager
         */
        void shutdown();

        /**
         * @brief Creates a new entity
         * @return Pointer to the created entity
         */
        Entity *createEntity();

        /**
         * @brief Destroys an entity
         * @param entity Pointer to the entity
         */
        void destroyEntity(Entity *entity);

        /**
         * @brief Gets an entity by ID
         * @param id Entity ID
         * @return Pointer to the entity, or nullptr if not found
         */
        Entity *getEntity(uint32_t id);

        /**
         * @brief Adds a system to the entity manager
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

            // Add system to the list
            systems.push_back(std::move(system));

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

            for (auto &system : systems)
            {
                T *typedSystem = dynamic_cast<T *>(system.get());
                if (typedSystem)
                {
                    return typedSystem;
                }
            }

            return nullptr;
        }

    private:
        /**
         * @brief Reference to the engine
         */
        Engine &engine;

        /**
         * @brief Map of entities
         */
        std::unordered_map<uint32_t, std::unique_ptr<Entity>> entities;

        /**
         * @brief List of systems
         */
        std::vector<std::unique_ptr<System>> systems;

        /**
         * @brief Queue of entity IDs to be reused
         */
        std::queue<uint32_t> freeIds;

        /**
         * @brief Next entity ID
         */
        uint32_t nextId;
    };

} // namespace Engine