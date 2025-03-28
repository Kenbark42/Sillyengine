#pragma once

#include "Engine/Renderer/Window.hpp"

// Forward declarations for GLFW
struct GLFWwindow;

namespace Engine
{
    /**
     * @brief OpenGL window implementation using GLFW
     */
    class OpenGLWindow : public Window
    {
    public:
        /**
         * @brief Constructor
         * @param width Window width
         * @param height Window height
         * @param title Window title
         */
        OpenGLWindow(int width, int height, const std::string &title);

        /**
         * @brief Destructor
         */
        ~OpenGLWindow() override;

        /**
         * @brief Initializes the window
         * @return True if initialization succeeded, false otherwise
         */
        bool initialize() override;

        /**
         * @brief Shuts down the window
         */
        void shutdown() override;

        /**
         * @brief Polls window events
         */
        void pollEvents() override;

        /**
         * @brief Swaps the window buffers
         */
        void swapBuffers() override;

        /**
         * @brief Checks if the window should close
         * @return True if the window should close, false otherwise
         */
        bool shouldClose() const override;

        /**
         * @brief Sets the window title
         * @param title New window title
         */
        void setTitle(const std::string &title) override;

        /**
         * @brief Gets the window's native handle
         * @return Native handle
         */
        void *getNativeHandle() const override;

        /**
         * @brief Gets the GLFW window handle
         * @return GLFW window handle
         */
        GLFWwindow *getGLFWWindow() const { return window; }

    private:
        /**
         * @brief GLFW window handle
         */
        GLFWwindow *window;

        /**
         * @brief Flag indicating if GLFW is initialized
         */
        static bool glfwInitialized;
    };

} // namespace Engine