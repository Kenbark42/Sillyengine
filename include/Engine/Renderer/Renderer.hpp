// include/Engine/Renderer/Renderer.hpp
#pragma once

#include <string>
#include <memory>
#include <vector>

#include "Engine/Core/Config.hpp"
#include "Engine/Math/Matrix.hpp"

namespace Engine
{

    class Window;
    class Camera;
    class Mesh;
    class Shader;
    class Material;
    class Texture;

    struct RendererConfig
    {
        bool vsync;
        bool msaa;
        int msaaSamples;
        bool hdr;
    };

    /**
     * @brief Abstract renderer interface
     *
     * The renderer is responsible for drawing meshes and handling the graphics context.
     */
    class Renderer
    {
    public:
        /**
         * @brief Constructor
         * @param config Renderer configuration
         */
        explicit Renderer(const RendererConfig &config);

        /**
         * @brief Virtual destructor
         */
        virtual ~Renderer() = default;

        /**
         * @brief Initializes the renderer
         * @param width Window width
         * @param height Window height
         * @param title Window title
         * @return True if initialization succeeded, false otherwise
         */
        virtual bool initialize(int width, int height, const std::string &title) = 0;

        /**
         * @brief Shuts down the renderer
         */
        virtual void shutdown() = 0;

        /**
         * @brief Begins a new frame
         */
        virtual void beginFrame() = 0;

        /**
         * @brief Ends the current frame
         */
        virtual void endFrame() = 0;

        /**
         * @brief Draws a mesh
         * @param mesh Mesh to draw
         * @param material Material to use
         * @param transform Model transformation matrix
         */
        virtual void drawMesh(Mesh *mesh, Material *material, const Matrix4 &transform) = 0;

        /**
         * @brief Sets the active camera
         * @param camera Camera to use for rendering
         */
        virtual void setCamera(Camera *camera) = 0;

        /**
         * @brief Gets the active camera
         * @return Pointer to the active camera
         */
        virtual Camera *getCamera() const = 0;

        /**
         * @brief Gets the window
         * @return Pointer to the window
         */
        virtual Window *getWindow() const = 0;

        /**
         * @brief Checks if the window should close
         * @return True if the window should close, false otherwise
         */
        virtual bool shouldClose() const = 0;

        /**
         * @brief Gets the renderer configuration
         * @return Renderer configuration
         */
        const RendererConfig &getConfig() const { return config; }

    protected:
        /**
         * @brief Renderer configuration
         */
        RendererConfig config;
    };

} // namespace Engine