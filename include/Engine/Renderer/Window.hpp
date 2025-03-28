// include/Engine/Renderer/Window.hpp
#pragma once

#include <string>
#include <functional>

namespace Engine
{
    /**
     * @brief Abstract window interface
     *
     * The window class encapsulates a platform-specific window.
     */
    class Window
    {
    public:
        /**
         * @brief Constructor
         * @param width Window width
         * @param height Window height
         * @param title Window title
         */
        Window(int width, int height, const std::string &title);

        /**
         * @brief Virtual destructor
         */
        virtual ~Window() = default;

        /**
         * @brief Initializes the window
         * @return True if initialization succeeded, false otherwise
         */
        virtual bool initialize() = 0;

        /**
         * @brief Shuts down the window
         */
        virtual void shutdown() = 0;

        /**
         * @brief Polls window events
         */
        virtual void pollEvents() = 0;

        /**
         * @brief Swaps the window buffers
         */
        virtual void swapBuffers() = 0;

        /**
         * @brief Checks if the window should close
         * @return True if the window should close, false otherwise
         */
        virtual bool shouldClose() const = 0;

        /**
         * @brief Gets the window width
         * @return Window width
         */
        int getWidth() const { return width; }

        /**
         * @brief Gets the window height
         * @return Window height
         */
        int getHeight() const { return height; }

        /**
         * @brief Gets the window title
         * @return Window title
         */
        const std::string &getTitle() const { return title; }

        /**
         * @brief Sets the window title
         * @param title New window title
         */
        virtual void setTitle(const std::string &title) = 0;

        /**
         * @brief Gets the window's native handle
         * @return Native handle
         */
        virtual void *getNativeHandle() const = 0;

    protected:
        /**
         * @brief Window width
         */
        int width;

        /**
         * @brief Window height
         */
        int height;

        /**
         * @brief Window title
         */
        std::string title;
    };

} // namespace Engine

// include/Engine/Renderer/OpenGLWindow.hpp
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

// src/Engine/Renderer/Window.cpp
#include "Engine/Renderer/Window.hpp"

namespace Engine
{

    Window::Window(int width, int height, const std::string &title)
        : width(width), height(height), title(title)
    {
    }

} // namespace Engine