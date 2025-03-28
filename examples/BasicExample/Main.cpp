#include "Engine/Core/Engine.hpp"
#include "Engine/ECS/Entity.hpp"
#include "Engine/ECS/Component.hpp"
#include "Engine/ECS/System.hpp"
#include "Engine/Scene/Scene.hpp"
#include "Engine/Renderer/Camera.hpp"

using namespace Engine;

// Custom component
class PlayerComponent : public ComponentT<PlayerComponent>
{
public:
    PlayerComponent(float moveSpeed = 5.0f, float rotateSpeed = 60.0f)
        : moveSpeed(moveSpeed), rotateSpeed(rotateSpeed) {}

    float moveSpeed;
    float rotateSpeed;
};

// Custom system
class PlayerSystem : public System
{
public:
    explicit PlayerSystem(Engine &engine) : System(engine)
    {
        name = "PlayerSystem";

        // Register required components
        requiredComponents.insert(std::type_index(typeid(PlayerComponent)));
    }

    bool initialize() override
    {
        return true;
    }

    void update(float deltaTime) override
    {
        auto &input = engine.getInputManager();

        for (auto entity : entities)
        {
            auto &player = entity->getComponent<PlayerComponent>();
            auto &transform = entity->getTransform();

            // Handle movement
            Vector3 movement;

            if (input.isKeyPressed(Key::W))
            {
                movement.z += 1.0f;
            }
            if (input.isKeyPressed(Key::S))
            {
                movement.z -= 1.0f;
            }
            if (input.isKeyPressed(Key::A))
            {
                movement.x -= 1.0f;
            }
            if (input.isKeyPressed(Key::D))
            {
                movement.x += 1.0f;
            }

            // Normalize and apply movement
            if (movement.lengthSquared() > 0.0f)
            {
                movement.normalize();
                movement = movement * player.moveSpeed * deltaTime;
                transform.translate(movement);
            }

            // Handle rotation
            Vector3 rotation;

            if (input.isKeyPressed(Key::Q))
            {
                rotation.y -= 1.0f;
            }
            if (input.isKeyPressed(Key::E))
            {
                rotation.y += 1.0f;
            }

            // Apply rotation
            if (rotation.lengthSquared() > 0.0f)
            {
                rotation = rotation * player.rotateSpeed * deltaTime;
                transform.rotate(rotation);
            }
        }
    }

    void shutdown() override
    {
        // Nothing to clean up
    }
};

int main()
{
    // Create engine with default configuration
    Config config;
    config.windowWidth = 1280;
    config.windowHeight = 720;
    config.windowTitle = "Game Engine Example";

    Engine engine(config);

    // Initialize the engine
    if (!engine.initialize())
    {
        Logger::error("Failed to initialize engine");
        return -1;
    }

    // Create a scene
    auto scene = std::make_unique<Scene>("MainScene");

    // Add a player entity
    auto player = scene->createEntity();
    player->setName("Player");
    player->getTransform().setPosition(0.0f, 0.0f, 0.0f);
    player->addComponent<PlayerComponent>(5.0f, 60.0f);

    // Add a camera entity
    auto camera = scene->createEntity();
    camera->setName("MainCamera");
    camera->getTransform().setPosition(0.0f, 2.0f, -5.0f);
    camera->getTransform().setRotation(15.0f, 0.0f, 0.0f);
    auto &cameraComponent = camera->addComponent<CameraComponent>();
    cameraComponent.setFov(60.0f);

    // Set the camera as active
    engine.getRenderer().setCamera(&cameraComponent);

    // Add a player system
    scene->addSystem<PlayerSystem>();

    // Set the scene as active
    engine.getSceneManager().setActiveScene(scene.get());

    // Run the engine
    int result = engine.run();

    return result;
}