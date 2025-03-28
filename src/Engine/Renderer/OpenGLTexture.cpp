// src/Engine/Renderer/OpenGLTexture.cpp
#include "Engine/Renderer/OpenGLTexture.hpp"
#include "Engine/Core/Logger.hpp"

#include <glad/glad.h>
#include <stb_image.h>

namespace Engine
{

    OpenGLTexture::OpenGLTexture()
        : textureId(0)
    {
        // Create texture
        glGenTextures(1, &textureId);
    }

    OpenGLTexture::~OpenGLTexture()
    {
        if (textureId)
        {
            glDeleteTextures(1, &textureId);
        }
    }

    bool OpenGLTexture::load(const std::string &filepath)
    {
        // Load image
        int channels;
        unsigned char *data = stbi_load(filepath.c_str(), &width, &height, &channels, 0);
        if (!data)
        {
            Logger::error("Failed to load texture: " + filepath);
            return false;
        }

        // Determine format
        if (channels == 3)
        {
            format = TextureFormat::RGB;
        }
        else if (channels == 4)
        {
            format = TextureFormat::RGBA;
        }
        else
        {
            Logger::error("Unsupported texture format: " + std::to_string(channels) + " channels");
            stbi_image_free(data);
            return false;
        }

        // Create texture
        bool result = create(width, height, data, format);

        // Free image data
        stbi_image_free(data);

        return result;
    }

    bool OpenGLTexture::create(int width, int height, unsigned char *data, TextureFormat format)
    {
        this->width = width;
        this->height = height;
        this->format = format;

        // Bind texture
        glBindTexture(GL_TEXTURE_2D, textureId);

        // Set texture data
        unsigned int glFormat = getGLFormat(format);
        unsigned int glInternalFormat = getGLFormat(format, true);
        glTexImage2D(GL_TEXTURE_2D, 0, glInternalFormat, width, height, 0, glFormat, GL_UNSIGNED_BYTE, data);

        // Set default filtering and wrapping
        setFilter(TextureFilter::LinearMipmap, TextureFilter::Linear);
        setWrap(TextureWrap::Repeat, TextureWrap::Repeat);

        // Generate mipmaps
        generateMipmaps();

        // Unbind texture
        glBindTexture(GL_TEXTURE_2D, 0);

        return true;
    }

    void OpenGLTexture::bind(int unit) const
    {
        glActiveTexture(GL_TEXTURE0 + unit);
        glBindTexture(GL_TEXTURE_2D, textureId);
    }

    void OpenGLTexture::unbind(int unit) const
    {
        glActiveTexture(GL_TEXTURE0 + unit);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void OpenGLTexture::setFilter(TextureFilter filterMin, TextureFilter filterMag)
    {
        // Bind texture
        glBindTexture(GL_TEXTURE_2D, textureId);

        // Set filtering
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, getGLFilter(filterMin, true));
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, getGLFilter(filterMag, false));

        // Unbind texture
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void OpenGLTexture::setWrap(TextureWrap wrapS, TextureWrap wrapT)
    {
        // Bind texture
        glBindTexture(GL_TEXTURE_2D, textureId);

        // Set wrapping
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, getGLWrap(wrapS));
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, getGLWrap(wrapT));

        // Unbind texture
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void OpenGLTexture::generateMipmaps()
    {
        // Bind texture
        glBindTexture(GL_TEXTURE_2D, textureId);

        // Generate mipmaps
        glGenerateMipmap(GL_TEXTURE_2D);

        // Unbind texture
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    unsigned int OpenGLTexture::getGLFilter(TextureFilter filter, bool mipmap) const
    {
        switch (filter)
        {
        case TextureFilter::Nearest:
            return GL_NEAREST;
        case TextureFilter::Linear:
            return GL_LINEAR;
        case TextureFilter::NearestMipmap:
            return mipmap ? GL_NEAREST_MIPMAP_NEAREST : GL_NEAREST;
        case TextureFilter::LinearMipmap:
            return mipmap ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR;
        default:
            return GL_LINEAR;
        }
    }

    unsigned int OpenGLTexture::getGLWrap(TextureWrap wrap) const
    {
        switch (wrap)
        {
        case TextureWrap::Repeat:
            return GL_REPEAT;
        case TextureWrap::MirroredRepeat:
            return GL_MIRRORED_REPEAT;
        case TextureWrap::ClampToEdge:
            return GL_CLAMP_TO_EDGE;
        case TextureWrap::ClampToBorder:
            return GL_CLAMP_TO_BORDER;
        default:
            return GL_REPEAT;
        }
    }

    unsigned int OpenGLTexture::getGLFormat(TextureFormat format, bool internal) const
    {
        switch (format)
        {
        case TextureFormat::RGB:
            return internal ? GL_RGB8 : GL_RGB;
        case TextureFormat::RGBA:
            return internal ? GL_RGBA8 : GL_RGBA;
        case TextureFormat::Depth:
            return internal ? GL_DEPTH_COMPONENT24 : GL_DEPTH_COMPONENT;
        default:
            return GL_RGBA;
        }
    }

} // namespace Engine

// Add these factory methods to ResourceManager class
std::unique_ptr<Texture> ResourceManager::createTexture()
{
    return std::make_unique<OpenGLTexture>();
}

std::unique_ptr<Shader> ResourceManager::createShader(const std::string &name)
{
    return std::make_unique<OpenGLShader>(name);
}

// This method would typically use a library like Assimp to load models
// For simplicity, this is a placeholder
std::unique_ptr<Mesh> ResourceManager::createMesh(const std::string &name)
{
    return std::make_unique<OpenGLMesh>(name);
}

bool ResourceManager::loadMeshFromFile(Mesh *mesh, const std::string &filepath)
{
    // This would typically use a model loading library like Assimp
    // For now, just a placeholder
    Logger::warning("Mesh loading from file not implemented yet: " + filepath);
    return false;
}