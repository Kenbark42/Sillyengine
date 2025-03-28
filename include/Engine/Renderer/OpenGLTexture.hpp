#pragma once

#include "Engine/Renderer/Texture.hpp"

namespace Engine
{
    /**
     * @brief OpenGL implementation of the texture
     */
    class OpenGLTexture : public Texture
    {
    public:
        /**
         * @brief Constructor
         */
        OpenGLTexture();

        /**
         * @brief Destructor
         */
        ~OpenGLTexture() override;

        /**
         * @brief Loads a texture from file
         * @param filepath Path to the texture file
         * @return True if loading succeeded, false otherwise
         */
        bool load(const std::string &filepath) override;

        /**
         * @brief Creates a texture from data
         * @param width Texture width
         * @param height Texture height
         * @param data Texture data
         * @param format Texture format
         * @return True if creation succeeded, false otherwise
         */
        bool create(int width, int height, unsigned char *data, TextureFormat format = TextureFormat::RGBA) override;

        /**
         * @brief Binds the texture
         * @param unit Texture unit
         */
        void bind(int unit = 0) const override;

        /**
         * @brief Unbinds the texture
         * @param unit Texture unit
         */
        void unbind(int unit = 0) const override;

        /**
         * @brief Sets the texture filtering mode
         * @param filterMin Filter for minification
         * @param filterMag Filter for magnification
         */
        void setFilter(TextureFilter filterMin, TextureFilter filterMag) override;

        /**
         * @brief Sets the texture wrap mode
         * @param wrapS Wrap mode for S coordinate
         * @param wrapT Wrap mode for T coordinate
         */
        void setWrap(TextureWrap wrapS, TextureWrap wrapT) override;

        /**
         * @brief Generates mipmaps
         */
        void generateMipmaps() override;

        /**
         * @brief Gets the OpenGL texture ID
         * @return OpenGL texture ID
         */
        unsigned int getTextureId() const { return textureId; }

    private:
        /**
         * @brief Converts a TextureFilter to an OpenGL filter mode
         * @param filter Texture filter mode
         * @param mipmap Is it a mipmap filter?
         * @return OpenGL filter mode
         */
        unsigned int getGLFilter(TextureFilter filter, bool mipmap) const;

        /**
         * @brief Converts a TextureWrap to an OpenGL wrap mode
         * @param wrap Texture wrap mode
         * @return OpenGL wrap mode
         */
        unsigned int getGLWrap(TextureWrap wrap) const;

        /**
         * @brief Converts a TextureFormat to an OpenGL format
         * @param format Texture format
         * @param internal Return internal format
         * @return OpenGL format
         */
        unsigned int getGLFormat(TextureFormat format, bool internal = false) const;

        /**
         * @brief OpenGL texture ID
         */
        unsigned int textureId;
    };

} // namespace Engine