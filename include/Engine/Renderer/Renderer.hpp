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

// include/Engine/Renderer/OpenGLRenderer.hpp
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

// include/Engine/Renderer/Shader.hpp
#pragma once

#include <string>
#include <unordered_map>

#include "Engine/Math/Vector.hpp"
#include "Engine/Math/Matrix.hpp"

namespace Engine
{

    /**
     * @brief Abstract shader interface
     *
     * The shader class encapsulates a shader program.
     */
    class Shader
    {
    public:
        /**
         * @brief Constructor
         * @param name Shader name
         */
        explicit Shader(const std::string &name);

        /**
         * @brief Virtual destructor
         */
        virtual ~Shader() = default;

        /**
         * @brief Compiles the shader
         * @param vertexSource Vertex shader source code
         * @param fragmentSource Fragment shader source code
         * @return True if compilation succeeded, false otherwise
         */
        virtual bool compile(const std::string &vertexSource, const std::string &fragmentSource) = 0;

        /**
         * @brief Binds the shader
         */
        virtual void bind() const = 0;

        /**
         * @brief Unbinds the shader
         */
        virtual void unbind() const = 0;

        /**
         * @brief Sets a uniform value
         * @param name Uniform name
         * @param value Uniform value
         */
        virtual void setInt(const std::string &name, int value) = 0;

        /**
         * @brief Sets a uniform value
         * @param name Uniform name
         * @param value Uniform value
         */
        virtual void setFloat(const std::string &name, float value) = 0;

        /**
         * @brief Sets a uniform value
         * @param name Uniform name
         * @param value Uniform value
         */
        virtual void setVector2(const std::string &name, const Vector2 &value) = 0;

        /**
         * @brief Sets a uniform value
         * @param name Uniform name
         * @param value Uniform value
         */
        virtual void setVector3(const std::string &name, const Vector3 &value) = 0;

        /**
         * @brief Sets a uniform value
         * @param name Uniform name
         * @param value Uniform value
         */
        virtual void setVector4(const std::string &name, const Vector4 &value) = 0;

        /**
         * @brief Sets a uniform value
         * @param name Uniform name
         * @param value Uniform value
         */
        virtual void setMatrix3(const std::string &name, const Matrix3 &value) = 0;

        /**
         * @brief Sets a uniform value
         * @param name Uniform name
         * @param value Uniform value
         */
        virtual void setMatrix4(const std::string &name, const Matrix4 &value) = 0;

        /**
         * @brief Gets the shader name
         * @return Shader name
         */
        const std::string &getName() const { return name; }

    protected:
        /**
         * @brief Shader name
         */
        std::string name;
    };

} // namespace Engine

// include/Engine/Renderer/Mesh.hpp
#pragma once

#include <vector>
#include <string>
#include <memory>

#include "Engine/Math/Vector.hpp"

namespace Engine
{

    /**
     * @brief Vertex data structure
     */
    struct Vertex
    {
        Vector3 position;
        Vector3 normal;
        Vector2 texCoord;
        Vector3 tangent;
        Vector3 bitangent;
    };

    /**
     * @brief Abstract mesh interface
     *
     * The mesh class encapsulates vertex and index data.
     */
    class Mesh
    {
    public:
        /**
         * @brief Constructor
         * @param name Mesh name
         */
        explicit Mesh(const std::string &name);

        /**
         * @brief Virtual destructor
         */
        virtual ~Mesh() = default;

        /**
         * @brief Sets the vertex data
         * @param vertices Vector of vertices
         */
        virtual void setVertices(const std::vector<Vertex> &vertices) = 0;

        /**
         * @brief Sets the index data
         * @param indices Vector of indices
         */
        virtual void setIndices(const std::vector<uint32_t> &indices) = 0;

        /**
         * @brief Builds the mesh
         * @return True if building succeeded, false otherwise
         */
        virtual bool build() = 0;

        /**
         * @brief Binds the mesh
         */
        virtual void bind() const = 0;

        /**
         * @brief Unbinds the mesh
         */
        virtual void unbind() const = 0;

        /**
         * @brief Draws the mesh
         */
        virtual void draw() const = 0;

        /**
         * @brief Gets the mesh name
         * @return Mesh name
         */
        const std::string &getName() const { return name; }

        /**
         * @brief Gets the number of vertices
         * @return Number of vertices
         */
        size_t getVertexCount() const { return vertexCount; }

        /**
         * @brief Gets the number of indices
         * @return Number of indices
         */
        size_t getIndexCount() const { return indexCount; }

    protected:
        /**
         * @brief Mesh name
         */
        std::string name;

        /**
         * @brief Number of vertices
         */
        size_t vertexCount;

        /**
         * @brief Number of indices
         */
        size_t indexCount;
    };

} // namespace Engine

// include/Engine/Renderer/Camera.hpp
#pragma once

#include "Engine/Math/Vector.hpp"
#include "Engine/Math/Matrix.hpp"
#include "Engine/ECS/Component.hpp"

namespace Engine
{

    /**
     * @brief Camera component
     *
     * The camera component provides a view into the 3D world.
     */
    class CameraComponent : public ComponentT<CameraComponent>
    {
    public:
        /**
         * @brief Projection type
         */
        enum class ProjectionType
        {
            Perspective,
            Orthographic
        };

        /**
         * @brief Constructor
         * @param fov Field of view (in degrees)
         * @param aspectRatio Aspect ratio (width / height)
         * @param nearPlane Near clipping plane
         * @param farPlane Far clipping plane
         */
        CameraComponent(float fov = 45.0f, float aspectRatio = 16.0f / 9.0f, float nearPlane = 0.1f, float farPlane = 1000.0f);

        /**
         * @brief Gets the view matrix
         * @return View matrix
         */
        const Matrix4 &getViewMatrix() const { return viewMatrix; }

        /**
         * @brief Gets the projection matrix
         * @return Projection matrix
         */
        const Matrix4 &getProjectionMatrix() const { return projectionMatrix; }

        /**
         * @brief Gets the view-projection matrix
         * @return View-projection matrix
         */
        const Matrix4 &getViewProjectionMatrix() const { return viewProjectionMatrix; }

        /**
         * @brief Updates the camera matrices
         */
        void updateMatrices();

        /**
         * @brief Sets the projection type
         * @param type Projection type
         */
        void setProjectionType(ProjectionType type);

        /**
         * @brief Gets the projection type
         * @return Projection type
         */
        ProjectionType getProjectionType() const { return projectionType; }

        /**
         * @brief Sets the field of view
         * @param fov Field of view (in degrees)
         */
        void setFov(float fov)
        {
            this->fov = fov;
            recalculateProjection = true;
        }

        /**
         * @brief Gets the field of view
         * @return Field of view (in degrees)
         */
        float getFov() const { return fov; }

        /**
         * @brief Sets the aspect ratio
         * @param aspectRatio Aspect ratio (width / height)
         */
        void setAspectRatio(float aspectRatio)
        {
            this->aspectRatio = aspectRatio;
            recalculateProjection = true;
        }

        /**
         * @brief Gets the aspect ratio
         * @return Aspect ratio (width / height)
         */
        float getAspectRatio() const { return aspectRatio; }

        /**
         * @brief Sets the near clipping plane
         * @param nearPlane Near clipping plane
         */
        void setNearPlane(float nearPlane)
        {
            this->nearPlane = nearPlane;
            recalculateProjection = true;
        }

        /**
         * @brief Gets the near clipping plane
         * @return Near clipping plane
         */
        float getNearPlane() const { return nearPlane; }

        /**
         * @brief Sets the far clipping plane
         * @param farPlane Far clipping plane
         */
        void setFarPlane(float farPlane)
        {
            this->farPlane = farPlane;
            recalculateProjection = true;
        }

        /**
         * @brief Gets the far clipping plane
         * @return Far clipping plane
         */
        float getFarPlane() const { return farPlane; }

        /**
         * @brief Sets the orthographic size
         * @param size Orthographic size
         */
        void setOrthographicSize(float size)
        {
            this->orthographicSize = size;
            recalculateProjection = true;
        }

        /**
         * @brief Gets the orthographic size
         * @return Orthographic size
         */
        float getOrthographicSize() const { return orthographicSize; }

    private:
        /**
         * @brief Updates the projection matrix
         */
        void updateProjectionMatrix();

        /**
         * @brief Updates the view matrix
         */
        void updateViewMatrix();

        /**
         * @brief Projection type
         */
        ProjectionType projectionType;

        /**
         * @brief Field of view (in degrees)
         */
        float fov;

        /**
         * @brief Aspect ratio (width / height)
         */
        float aspectRatio;

        /**
         * @brief Near clipping plane
         */
        float nearPlane;

        /**
         * @brief Far clipping plane
         */
        float farPlane;

        /**
         * @brief Orthographic size
         */
        float orthographicSize;

        /**
         * @brief View matrix
         */
        Matrix4 viewMatrix;

        /**
         * @brief Projection matrix
         */
        Matrix4 projectionMatrix;

        /**
         * @brief View-projection matrix
         */
        Matrix4 viewProjectionMatrix;

        /**
         * @brief Flag indicating if the projection matrix needs to be recalculated
         */
        bool recalculateProjection;
    };

} // namespace Engine