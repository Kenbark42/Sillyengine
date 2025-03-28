#pragma once

#include <string>
#include "Engine/Core/Logger.hpp"
#include "Engine/Renderer/Renderer.hpp"

namespace Engine
{

    /**
     * @brief Physics configuration
     */
    struct PhysicsConfig
    {
        /**
         * @brief Gravity vector
         */
        float gravityX = 0.0f;
        float gravityY = -9.81f;
        float gravityZ = 0.0f;

        /**
         * @brief Fixed timestep for physics simulation
         */
        float fixedTimestep = 1.0f / 60.0f;

        /**
         * @brief Maximum number of substeps per frame
         */
        int maxSubsteps = 10;

        /**
         * @brief Collision detection margin
         */
        float collisionMargin = 0.04f;

        /**
         * @brief Sleep threshold for rigid bodies
         */
        float sleepThreshold = 0.8f;

        /**
         * @brief Default friction
         */
        float defaultFriction = 0.5f;

        /**
         * @brief Default restitution
         */
        float defaultRestitution = 0.0f;

        /**
         * @brief Default linear damping
         */
        float defaultLinearDamping = 0.0f;

        /**
         * @brief Default angular damping
         */
        float defaultAngularDamping = 0.0f;
    };

    /**
     * @brief Audio configuration
     */
    struct AudioConfig
    {
        /**
         * @brief Master volume
         */
        float masterVolume = 1.0f;

        /**
         * @brief Music volume
         */
        float musicVolume = 1.0f;

        /**
         * @brief SFX volume
         */
        float sfxVolume = 1.0f;

        /**
         * @brief Maximum number of sound sources
         */
        int maxSources = 32;

        /**
         * @brief Doppler factor
         */
        float dopplerFactor = 1.0f;

        /**
         * @brief Speed of sound
         */
        float speedOfSound = 343.3f;

        /**
         * @brief Distance model
         */
        enum class DistanceModel
        {
            Inverse,
            InverseClamped,
            Linear,
            LinearClamped,
            Exponent,
            ExponentClamped
        };

        /**
         * @brief Distance model
         */
        DistanceModel distanceModel = DistanceModel::InverseClamped;
    };

    /**
     * @brief Resource configuration
     */
    struct ResourceConfig
    {
        /**
         * @brief Path to the resources directory
         */
        std::string resourcesPath = "resources";

        /**
         * @brief Maximum number of resources in the cache
         */
        int maxCacheSize = 1024;

        /**
         * @brief Automatic resource reloading
         */
        bool autoReload = false;
    };

    /**
     * @brief Engine configuration
     */
    struct Config
    {
        /**
         * @brief Window width
         */
        int windowWidth = 1280;

        /**
         * @brief Window height
         */
        int windowHeight = 720;

        /**
         * @brief Window title
         */
        std::string windowTitle = "Game Engine";

        /**
         * @brief Log level
         */
        LogLevel logLevel = LogLevel::Info;

        /**
         * @brief Renderer configuration
         */
        RendererConfig renderer;

        /**
         * @brief Physics configuration
         */
        PhysicsConfig physics;

        /**
         * @brief Audio configuration
         */
        AudioConfig audio;

        /**
         * @brief Resource configuration
         */
        ResourceConfig resource;

        /**
         * @brief Target frame rate (0 for uncapped)
         */
        int targetFrameRate = 0;

        /**
         * @brief Fixed update rate for gameplay logic
         */
        float fixedUpdateRate = 1.0f / 60.0f;

        /**
         * @brief Constructor with default values
         */
        Config()
        {
            // Set default renderer configuration
            renderer.vsync = true;
            renderer.msaa = true;
            renderer.msaaSamples = 4;
            renderer.hdr = true;
        }
    };

} // namespace Engine