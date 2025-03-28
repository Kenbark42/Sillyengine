#pragma once

#include "Engine/Renderer/Shader.hpp"

namespace Engine
{

    /**
     * @brief OpenGL implementation of the shader
     */
    class OpenGLShader : public Shader
    {
    public:
        /**
         * @brief Constructor
         * @param name Shader name
         */
        explicit OpenGLShader(const std::string &name);

        /**
         * @brief Destructor
         */
        ~OpenGLShader() override;

        /**
         * @brief Compiles the shader
         * @param vertexSource Vertex shader source code
         * @param fragmentSource Fragment shader source code
         * @return True if compilation succeeded, false otherwise
         */
        bool compile(const std::string &vertexSource, const std::string &fragmentSource) override;

        /**
         * @brief Binds the shader
         */
        void bind() const override;

        /**
         * @brief Unbinds the shader
         */
        void unbind() const override;

        /**
         * @brief Sets a uniform value
         * @param name Uniform name
         * @param value Uniform value
         */
        void setInt(const std::string &name, int value) override;

        /**
         * @brief Sets a uniform value
         * @param name Uniform name
         * @param value Uniform value
         */
        void setFloat(const std::string &name, float value) override;

        /**
         * @brief Sets a uniform value
         * @param name Uniform name
         * @param value Uniform value
         */
        void setVector2(const std::string &name, const Vector2 &value) override;

        /**
         * @brief Sets a uniform value
         * @param name Uniform name
         * @param value Uniform value
         */
        void setVector3(const std::string &name, const Vector3 &value) override;

        /**
         * @brief Sets a uniform value
         * @param name Uniform name
         * @param value Uniform value
         */
        void setVector4(const std::string &name, const Vector4 &value) override;

        /**
         * @brief Sets a uniform value
         * @param name Uniform name
         * @param value Uniform value
         */
        void setMatrix3(const std::string &name, const Matrix3 &value) override;

        /**
         * @brief Sets a uniform value
         * @param name Uniform name
         * @param value Uniform value
         */
        void setMatrix4(const std::string &name, const Matrix4 &value) override;

    private:
        /**
         * @brief Gets the location of a uniform
         * @param name Uniform name
         * @return Uniform location
         */
        int getUniformLocation(const std::string &name);

        /**
         * @brief Shader program ID
         */
        uint32_t programId;

        /**
         * @brief Cache of uniform locations
         */
        std::unordered_map<std::string, int> uniformLocationCache;
    };

} // namespace Engine