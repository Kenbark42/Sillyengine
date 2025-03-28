#include "Engine/Renderer/Material.hpp"
#include "Engine/Renderer/Shader.hpp"
#include "Engine/Renderer/Texture.hpp"
#include "Engine/Core/Logger.hpp"

namespace Engine
{

    Material::Material(const std::string &name, Shader *shader)
        : name(name), shader(shader)
    {
    }

    Material::~Material()
    {
    }

    void Material::bind()
    {
        if (!shader)
        {
            Logger::error("Material '" + name + "' has no shader");
            return;
        }

        // Bind shader
        shader->bind();

        // Set float parameters
        for (const auto &param : floatParams)
        {
            shader->setFloat(param.first, param.second);
        }

        // Set int parameters
        for (const auto &param : intParams)
        {
            shader->setInt(param.first, param.second);
        }

        // Set Vector2 parameters
        for (const auto &param : vec2Params)
        {
            shader->setVector2(param.first, param.second);
        }

        // Set Vector3 parameters
        for (const auto &param : vec3Params)
        {
            shader->setVector3(param.first, param.second);
        }

        // Set Vector4 parameters
        for (const auto &param : vec4Params)
        {
            shader->setVector4(param.first, param.second);
        }

        // Bind textures
        for (const auto &param : textureParams)
        {
            if (param.second.texture)
            {
                param.second.texture->bind(param.second.unit);
                shader->setInt(param.first, param.second.unit);
            }
        }
    }

    void Material::unbind()
    {
        // Unbind textures in reverse order
        for (auto it = textureParams.rbegin(); it != textureParams.rend(); ++it)
        {
            if (it->second.texture)
            {
                it->second.texture->unbind(it->second.unit);
            }
        }

        // Unbind shader
        if (shader)
        {
            shader->unbind();
        }
    }

    void Material::setFloat(const std::string &name, float value)
    {
        floatParams[name] = value;
    }

    void Material::setInt(const std::string &name, int value)
    {
        intParams[name] = value;
    }

    void Material::setVector2(const std::string &name, const Vector2 &value)
    {
        vec2Params[name] = value;
    }

    void Material::setVector3(const std::string &name, const Vector3 &value)
    {
        vec3Params[name] = value;
    }

    void Material::setVector4(const std::string &name, const Vector4 &value)
    {
        vec4Params[name] = value;
    }

    void Material::setTexture(const std::string &name, Texture *texture, int unit)
    {
        textureParams[name] = {texture, unit};
    }

} // namespace Engine