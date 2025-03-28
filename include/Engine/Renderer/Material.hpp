#pragma once

#include <string>
#include <unordered_map>
#include "Engine/Math/Vector.hpp"

namespace Engine
{
    class Shader;
    class Texture;

    /**
     * @brief Material class
     *
     * Materials define the appearance of objects by combining a shader with
     * parameters and textures.
     */
    class Material
    {
    public:
        /**
         * @brief Constructor
         * @param name Material name
         * @param shader Shader to use
         */
        Material(const std::string &name, Shader *shader);

        /**
         * @brief Destructor
         */
        ~Material();

        /**
         * @brief Gets the material name
         * @return Material name
         */
        const std::string &getName() const { return name; }

        /**
         * @brief Gets the shader
         * @return Pointer to the shader
         */
        Shader *getShader() const { return shader; }

        /**
         * @brief Sets the shader
         * @param shader Shader to use
         */
        void setShader(Shader *shader) { this->shader = shader; }

        /**
         * @brief Binds the material
         */
        void bind();

        /**
         * @brief Unbinds the material
         */
        void unbind();

        /**
         * @brief Sets a parameter
         * @param name Parameter name
         * @param value Parameter value
         */
        void setFloat(const std::string &name, float value);

        /**
         * @brief Sets a parameter
         * @param name Parameter name
         * @param value Parameter value
         */
        void setInt(const std::string &name, int value);

        /**
         * @brief Sets a parameter
         * @param name Parameter name
         * @param value Parameter value
         */
        void setVector2(const std::string &name, const Vector2 &value);

        /**
         * @brief Sets a parameter
         * @param name Parameter name
         * @param value Parameter value
         */
        void setVector3(const std::string &name, const Vector3 &value);

        /**
         * @brief Sets a parameter
         * @param name Parameter name
         * @param value Parameter value
         */
        void setVector4(const std::string &name, const Vector4 &value);

        /**
         * @brief Sets a texture
         * @param name Texture name
         * @param texture Texture to use
         * @param unit Texture unit
         */
        void setTexture(const std::string &name, Texture *texture, int unit = 0);

    private:
        /**
         * @brief Material name
         */
        std::string name;

        /**
         * @brief Shader to use
         */
        Shader *shader;

        /**
         * @brief Map of float parameters
         */
        std::unordered_map<std::string, float> floatParams;

        /**
         * @brief Map of int parameters
         */
        std::unordered_map<std::string, int> intParams;

        /**
         * @brief Map of Vector2 parameters
         */
        std::unordered_map<std::string, Vector2> vec2Params;

        /**
         * @brief Map of Vector3 parameters
         */
        std::unordered_map<std::string, Vector3> vec3Params;

        /**
         * @brief Map of Vector4 parameters
         */
        std::unordered_map<std::string, Vector4> vec4Params;

        /**
         * @brief Structure for texture parameters
         */
        struct TextureParam
        {
            Texture *texture;
            int unit;
        };

        /**
         * @brief Map of texture parameters
         */
        std::unordered_map<std::string, TextureParam> textureParams;
    };

} // namespace Engine