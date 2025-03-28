#include "Engine/Renderer/OpenGLRenderer.hpp"
#include "Engine/Renderer/OpenGLWindow.hpp"
#include "Engine/Core/Logger.hpp"
#include "Engine/Renderer/Camera.hpp"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Engine
{

    OpenGLRenderer::OpenGLRenderer(const RendererConfig &config)
        : Renderer(config), window(nullptr), activeCamera(nullptr), currentFrameBuffer(0)
    {
    }

    OpenGLRenderer::~OpenGLRenderer()
    {
        if (window)
        {
            shutdown();
        }
    }

    bool OpenGLRenderer::initialize(int width, int height, const std::string &title)
    {
        // Create window
        window = std::make_unique<OpenGLWindow>(width, height, title);
        if (!window->initialize())
        {
            Logger::error("Failed to initialize OpenGL window");
            return false;
        }

        // Initialize GLAD
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            Logger::error("Failed to initialize GLAD");
            return false;
        }

        // Configure global OpenGL state
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glFrontFace(GL_CCW);

        // Enable blending
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // Enable MSAA if requested
        if (config.msaa)
        {
            glEnable(GL_MULTISAMPLE);
        }

        // Print OpenGL info
        Logger::info("OpenGL Vendor: " + std::string((const char *)glGetString(GL_VENDOR)));
        Logger::info("OpenGL Renderer: " + std::string((const char *)glGetString(GL_RENDERER)));
        Logger::info("OpenGL Version: " + std::string((const char *)glGetString(GL_VERSION)));
        Logger::info("GLSL Version: " + std::string((const char *)glGetString(GL_SHADING_LANGUAGE_VERSION)));

        // Compile default shaders
        if (!compileDefaultShaders())
        {
            Logger::error("Failed to compile default shaders");
            return false;
        }

        // Set default viewport
        glViewport(0, 0, width, height);

        Logger::info("OpenGL Renderer initialized successfully");
        return true;
    }

    void OpenGLRenderer::shutdown()
    {
        // Clear default shaders
        defaultShaders.clear();

        // Shutdown window
        if (window)
        {
            window->shutdown();
            window.reset();
        }
    }

    void OpenGLRenderer::beginFrame()
    {
        // Clear the color and depth buffers
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void OpenGLRenderer::endFrame()
    {
        // Swap buffers
        window->swapBuffers();
    }

    void OpenGLRenderer::drawMesh(Mesh *mesh, Material *material, const Matrix4 &transform)
    {
        if (!mesh || !material || !activeCamera)
        {
            return;
        }

        // Bind shader from material
        material->bind();

        // Set common uniforms
        material->getShader()->setMatrix4("model", transform);
        material->getShader()->setMatrix4("view", activeCamera->getViewMatrix());
        material->getShader()->setMatrix4("projection", activeCamera->getProjectionMatrix());

        // Bind mesh
        mesh->bind();

        // Draw mesh
        mesh->draw();

        // Unbind mesh
        mesh->unbind();

        // Unbind shader
        material->unbind();
    }

    void OpenGLRenderer::setCamera(Camera *camera)
    {
        activeCamera = camera;
    }

    Camera *OpenGLRenderer::getCamera() const
    {
        return activeCamera;
    }

    Window *OpenGLRenderer::getWindow() const
    {
        return window.get();
    }

    bool OpenGLRenderer::shouldClose() const
    {
        return window->shouldClose();
    }

    bool OpenGLRenderer::compileDefaultShaders()
    {
        // Define vertex shader source
        const std::string phongVertexShader = R"(
            #version 330 core
            layout (location = 0) in vec3 aPos;
            layout (location = 1) in vec3 aNormal;
            layout (location = 2) in vec2 aTexCoord;
            
            uniform mat4 model;
            uniform mat4 view;
            uniform mat4 projection;
            
            out vec3 FragPos;
            out vec3 Normal;
            out vec2 TexCoord;
            
            void main()
            {
                FragPos = vec3(model * vec4(aPos, 1.0));
                Normal = mat3(transpose(inverse(model))) * aNormal;
                TexCoord = aTexCoord;
                gl_Position = projection * view * vec4(FragPos, 1.0);
            }
        )";

        // Define fragment shader source
        const std::string phongFragmentShader = R"(
            #version 330 core
            out vec4 FragColor;
            
            in vec3 FragPos;
            in vec3 Normal;
            in vec2 TexCoord;
            
            uniform vec3 viewPos;
            uniform vec3 lightPos;
            uniform vec3 lightColor;
            uniform vec3 objectColor;
            uniform float ambientStrength;
            uniform float specularStrength;
            uniform float shininess;
            uniform sampler2D diffuseTexture;
            uniform bool hasTexture;
            
            void main()
            {
                // Ambient
                vec3 ambient = ambientStrength * lightColor;
                
                // Diffuse
                vec3 norm = normalize(Normal);
                vec3 lightDir = normalize(lightPos - FragPos);
                float diff = max(dot(norm, lightDir), 0.0);
                vec3 diffuse = diff * lightColor;
                
                // Specular
                vec3 viewDir = normalize(viewPos - FragPos);
                vec3 reflectDir = reflect(-lightDir, norm);
                float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
                vec3 specular = specularStrength * spec * lightColor;
                
                // Combine
                vec3 baseColor = hasTexture ? texture(diffuseTexture, TexCoord).rgb : objectColor;
                vec3 result = (ambient + diffuse + specular) * baseColor;
                FragColor = vec4(result, 1.0);
            }
        )";

        // Create Phong shader
        auto phongShader = std::make_unique<OpenGLShader>("Phong");
        if (!phongShader->compile(phongVertexShader, phongFragmentShader))
        {
            Logger::error("Failed to compile Phong shader");
            return false;
        }

        // Add to default shaders
        defaultShaders["Phong"] = std::move(phongShader);

        return true;
    }

} // namespace Engine