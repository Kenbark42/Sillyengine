#include "Engine/Scene/Scene.hpp"
#include "Engine/Core/Logger.hpp"
#include "Engine/Core/Engine.hpp"
#include "Engine/ECS/EntityManager.hpp"

namespace Engine
{

    Scene::Scene(const std::string &name, Engine &engine)
        : name(name), engine(engine)
    {
        // Create entity manager
        entityManager = std::make_unique<EntityManager>(engine);
        if (!entityManager->initialize())
        {
            Logger::error("Failed to initialize entity manager for scene: " + name);
        }

        Logger::info("Scene created: " + name);
    }

    Scene::~Scene()
    {
        // Clear systems
        systems.clear();

        // Shutdown entity manager
        if (entityManager)
        {
            entityManager->shutdown();
        }

        Logger::info("Scene destroyed: " + name);
    }

    void Scene::update(float deltaTime)
    {
        // Update entity manager
        entityManager->update(deltaTime);

        // Update systems
        for (auto &pair : systems)
        {
            pair.second->update(deltaTime);
        }
    }

    void Scene::render()
    {
        // Nothing to do here; rendering is handled by the renderer
        // Systems can register with the renderer to render their entities
    }

    Entity *Scene::createEntity()
    {
        return entityManager->createEntity();
    }

    Entity *Scene::createEntity(const std::string &name)
    {
        // Create entity
        Entity *entity = createEntity();
        if (!entity)
        {
            return nullptr;
        }

        // Set name
        entity->setName(name);

        // Add to name map
        entityNames[name] = entity;

        return entity;
    }

    void Scene::destroyEntity(Entity *entity)
    {
        if (!entity)
        {
            return;
        }

        // Remove from name map
        auto it = entityNames.find(entity->getName());
        if (it != entityNames.end() && it->second == entity)
        {
            entityNames.erase(it);
        }

        // Destroy entity
        entityManager->destroyEntity(entity);
    }

    Entity *Scene::getEntity(uint32_t id)
    {
        return entityManager->getEntity(id);
    }

    Entity *Scene::getEntityByName(const std::string &name)
    {
        auto it = entityNames.find(name);
        if (it != entityNames.end())
        {
            return it->second;
        }
        return nullptr;
    }

} // namespace Engine