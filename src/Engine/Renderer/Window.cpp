#include "Engine/Renderer/Window.hpp"

namespace Engine
{

    Window::Window(int width, int height, const std::string &title)
        : width(width), height(height), title(title)
    {
    }

} // namespace Engine

// src/Engine/Renderer/OpenGLWindow.cpp
#include "Engine/Renderer/OpenGLWindow.hpp"
#include "Engine/Core/Logger.hpp"

#include <GLFW/glfw3.h>

namespace Engine
{

    // Initialize static member
    bool OpenGLWindow::glfwInitialized = false;

    OpenGLWindow::OpenGLWindow(int width, int height, const std::string &title)
        : Window(width, height, title), window(nullptr)
    {
    }

    OpenGLWindow::~OpenGLWindow()
    {
        if (window)
        {
            shutdown();
        }
    }

    bool OpenGLWindow::initialize()
    {
        // Initialize GLFW if not already initialized
        if (!glfwInitialized)
        {
            if (!glfwInit())
            {
                Logger::error("Failed to initialize GLFW");
                return false;
            }
            glfwInitialized = true;
        }

        // Configure OpenGL context
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

        // Create window
        window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
        if (!window)
        {
            Logger::error("Failed to create GLFW window");
            return false;
        }

        // Make OpenGL context current
        glfwMakeContextCurrent(window);

        // Set up framebuffer size callback
        glfwSetFramebufferSizeCallback(window, [](GLFWwindow *window, int width, int height)
                                       { glViewport(0, 0, width, height); });

        // Set up window size callback
        glfwSetWindowSizeCallback(window, [](GLFWwindow *window, int width, int height)
                                  {
            // Get the user pointer and cast it to OpenGLWindow
            OpenGLWindow *openglWindow = static_cast<OpenGLWindow *>(glfwGetWindowUserPointer(window));
            if (openglWindow)
            {
                openglWindow->width = width;
                openglWindow->height = height;
            } });

        // Set user pointer to this instance
        glfwSetWindowUserPointer(window, this);

        // Enable vsync
        glfwSwapInterval(1);

        Logger::info("OpenGL Window created successfully");
        return true;
    }

    void OpenGLWindow::shutdown()
    {
        if (window)
        {
            glfwDestroyWindow(window);
            window = nullptr;
        }

        // Check if there are any remaining windows
        bool windowsExist = false;
        GLFWwindow *existingWindow = glfwGetCurrentContext();
        if (existingWindow)
        {
            windowsExist = true;
        }

        // Only terminate GLFW if there are no windows left
        if (!windowsExist && glfwInitialized)
        {
            glfwTerminate();
            glfwInitialized = false;
        }
    }

    void OpenGLWindow::pollEvents()
    {
        glfwPollEvents();
    }

    void OpenGLWindow::swapBuffers()
    {
        glfwSwapBuffers(window);
    }

    bool OpenGLWindow::shouldClose() const
    {
        return glfwWindowShouldClose(window);
    }

    void OpenGLWindow::setTitle(const std::string &title)
    {
        this->title = title;
        if (window)
        {
            glfwSetWindowTitle(window, title.c_str());
        }
    }

    void *OpenGLWindow::getNativeHandle() const
    {
        return window;
    }

} // namespace Engine