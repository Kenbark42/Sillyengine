#include "Engine/Scene/SceneManager.hpp"
#include "Engine/Scene/Scene.hpp"
#include "Engine/Core/Logger.hpp"

namespace Engine
{

    SceneManager::SceneManager(Engine &engine)
        : engine(engine), activeScene(nullptr)
    {
    }

    SceneManager::~SceneManager()
    {
        if (scenes.size() > 0)
        {
            shutdown();
        }
    }

    bool SceneManager::initialize()
    {
        Logger::info("Initializing scene manager...");
        return true;
    }

    void SceneManager::update(float deltaTime)
    {
        // Update active scene
        if (activeScene)
        {
            activeScene->update(deltaTime);
        }
    }

    void SceneManager::render()
    {
        // Render active scene
        if (activeScene)
        {
            activeScene->render();
        }
    }

    void SceneManager::shutdown()
    {
        Logger::info("Shutting down scene manager...");

        // Clear active scene
        activeScene = nullptr;

        // Clear all scenes
        scenes.clear();
    }

    Scene *SceneManager::createScene(const std::string &name)
    {
        // Check if scene already exists
        if (scenes.find(name) != scenes.end())
        {
            Logger::warning("Scene already exists: " + name);
            return scenes[name].get();
        }

        // Create scene
        auto scene = std::make_unique<Scene>(name, engine);
        Scene *scenePtr = scene.get();

        // Add to map
        scenes[name] = std::move(scene);
        Logger::info("Scene created: " + name);

        return scenePtr;
    }

    Scene *SceneManager::getScene(const std::string &name)
    {
        auto it = scenes.find(name);
        if (it != scenes.end())
        {
            return it->second.get();
        }
        return nullptr;
    }

    bool SceneManager::destroyScene(const std::string &name)
    {
        auto it = scenes.find(name);
        if (it == scenes.end())
        {
            return false;
        }

        // Check if it's the active scene
        if (activeScene == it->second.get())
        {
            activeScene = nullptr;
        }

        // Remove from map
        scenes.erase(it);
        Logger::info("Scene destroyed: " + name);

        return true;
    }

    void SceneManager::setActiveScene(Scene *scene)
    {
        // Check if scene is valid
        if (!scene)
        {
            Logger::error("Cannot set active scene: Scene is null");
            return;
        }

        // Find scene in map
        for (const auto &pair : scenes)
        {
            if (pair.second.get() == scene)
            {
                activeScene = scene;
                Logger::info("Active scene set to: " + scene->getName());
                return;
            }
        }

        Logger::error("Cannot set active scene: Scene not found");
    }

} // namespace Engine