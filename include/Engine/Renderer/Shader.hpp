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