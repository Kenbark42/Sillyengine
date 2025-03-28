#pragma once

#include "Engine/Renderer/Renderer.hpp"

#include <unordered_map>
#include <string>

namespace Engine
{

    class OpenGLWindow;
    class OpenGLShader;

    /**
     * @brief OpenGL implementation of the renderer
     *
     * This class provides an OpenGL implementation of the renderer interface.
     */
    class OpenGLRenderer : public Renderer
    {
    public:
        /**
         * @brief Constructor
         * @param config Renderer configuration
         */
        explicit OpenGLRenderer(const RendererConfig &config);

        /**
         * @brief Destructor
         */
        ~OpenGLRenderer() override;

        /**
         * @brief Initializes the renderer
         * @param width Window width
         * @param height Window height
         * @param title Window title
         * @return True if initialization succeeded, false otherwise
         */
        bool initialize(int width, int height, const std::string &title) override;

        /**
         * @brief Shuts down the renderer
         */
        void shutdown() override;

        /**
         * @brief Begins a new frame
         */
        void beginFrame() override;

        /**
         * @brief Ends the current frame
         */
        void endFrame() override;

        /**
         * @brief Draws a mesh
         * @param mesh Mesh to draw
         * @param material Material to use
         * @param transform Model transformation matrix
         */
        void drawMesh(Mesh *mesh, Material *material, const Matrix4 &transform) override;

        /**
         * @brief Sets the active camera
         * @param camera Camera to use for rendering
         */
        void setCamera(Camera *camera) override;

        /**
         * @brief Gets the active camera
         * @return Pointer to the active camera
         */
        Camera *getCamera() const override;

        /**
         * @brief Gets the window
         * @return Pointer to the window
         */
        Window *getWindow() const override;

        /**
         * @brief Checks if the window should close
         * @return True if the window should close, false otherwise
         */
        bool shouldClose() const override;

    private:
        /**
         * @brief Compiles default shaders
         * @return True if compilation succeeded, false otherwise
         */
        bool compileDefaultShaders();

        /**
         * @brief Pointer to the window
         */
        std::unique_ptr<OpenGLWindow> window;

        /**
         * @brief Active camera
         */
        Camera *activeCamera;

        /**
         * @brief Map of default shaders
         */
        std::unordered_map<std::string, std::unique_ptr<OpenGLShader>> defaultShaders;

        /**
         * @brief Current frame buffer
         */
        uint32_t currentFrameBuffer;
    };

} // namespace Engine