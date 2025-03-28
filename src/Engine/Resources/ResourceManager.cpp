#include "Engine/Resources/ResourceManager.hpp"
#include "Engine/Core/Logger.hpp"
#include "Engine/Renderer/Texture.hpp"
#include "Engine/Renderer/Mesh.hpp"
#include "Engine/Renderer/Shader.hpp"
#include "Engine/Renderer/Material.hpp"

#include <filesystem>
#include <fstream>
#include <sstream>

namespace Engine
{

    ResourceManager::ResourceManager()
    {
    }

    ResourceManager::~ResourceManager()
    {
        if (initialized)
        {
            shutdown();
        }
    }

    bool ResourceManager::initialize()
    {
        Logger::info("Initializing resource manager...");
        initialized = true;
        return true;
    }

    void ResourceManager::shutdown()
    {
        Logger::info("Shutting down resource manager...");

        // Clear all resources
        textures.clear();
        meshes.clear();
        shaders.clear();
        materials.clear();

        initialized = false;
    }

    void ResourceManager::setResourcesPath(const std::string &path)
    {
        resourcesPath = path;
        Logger::info("Resources path set to: " + resourcesPath);
    }

    std::string ResourceManager::getResourcePath(const std::string &relativePath) const
    {
        return resourcesPath + "/" + relativePath;
    }

    Texture *ResourceManager::getTexture(const std::string &name)
    {
        auto it = textures.find(name);
        if (it != textures.end())
        {
            return it->second.get();
        }
        return nullptr;
    }

    Mesh *ResourceManager::getMesh(const std::string &name)
    {
        auto it = meshes.find(name);
        if (it != meshes.end())
        {
            return it->second.get();
        }
        return nullptr;
    }

    Shader *ResourceManager::getShader(const std::string &name)
    {
        auto it = shaders.find(name);
        if (it != shaders.end())
        {
            return it->second.get();
        }
        return nullptr;
    }

    Material *ResourceManager::getMaterial(const std::string &name)
    {
        auto it = materials.find(name);
        if (it != materials.end())
        {
            return it->second.get();
        }
        return nullptr;
    }

    Texture *ResourceManager::loadTexture(const std::string &name, const std::string &filepath)
    {
        // Check if texture already exists
        if (textures.find(name) != textures.end())
        {
            Logger::warning("Texture '" + name + "' already exists");
            return textures[name].get();
        }

        // Create texture
        auto texture = createTexture();
        if (!texture->load(getResourcePath(filepath)))
        {
            Logger::error("Failed to load texture: " + filepath);
            return nullptr;
        }

        // Add to map
        textures[name] = std::move(texture);
        Logger::info("Loaded texture: " + name);
        return textures[name].get();
    }

    Mesh *ResourceManager::loadMesh(const std::string &name, const std::string &filepath)
    {
        // Check if mesh already exists
        if (meshes.find(name) != meshes.end())
        {
            Logger::warning("Mesh '" + name + "' already exists");
            return meshes[name].get();
        }

        // Create mesh
        auto mesh = createMesh(name);
        if (!loadMeshFromFile(mesh.get(), getResourcePath(filepath)))
        {
            Logger::error("Failed to load mesh: " + filepath);
            return nullptr;
        }

        // Add to map
        meshes[name] = std::move(mesh);
        Logger::info("Loaded mesh: " + name);
        return meshes[name].get();
    }

    Shader *ResourceManager::loadShader(const std::string &name, const std::string &vertexPath, const std::string &fragmentPath)
    {
        // Check if shader already exists
        if (shaders.find(name) != shaders.end())
        {
            Logger::warning("Shader '" + name + "' already exists");
            return shaders[name].get();
        }

        // Load shader sources
        std::string vertexSource;
        std::string fragmentSource;

        if (!loadFileToString(getResourcePath(vertexPath), vertexSource))
        {
            Logger::error("Failed to load vertex shader: " + vertexPath);
            return nullptr;
        }

        if (!loadFileToString(getResourcePath(fragmentPath), fragmentSource))
        {
            Logger::error("Failed to load fragment shader: " + fragmentPath);
            return nullptr;
        }

        // Create shader
        auto shader = createShader(name);
        if (!shader->compile(vertexSource, fragmentSource))
        {
            Logger::error("Failed to compile shader: " + name);
            return nullptr;
        }

        // Add to map
        shaders[name] = std::move(shader);
        Logger::info("Loaded shader: " + name);
        return shaders[name].get();
    }

    Material *ResourceManager::createMaterial(const std::string &name, Shader *shader)
    {
        // Check if material already exists
        if (materials.find(name) != materials.end())
        {
            Logger::warning("Material '" + name + "' already exists");
            return materials[name].get();
        }

        // Create material
        auto material = std::make_unique<Material>(name, shader);

        // Add to map
        materials[name] = std::move(material);
        Logger::info("Created material: " + name);
        return materials[name].get();
    }

    bool ResourceManager::loadFileToString(const std::string &filepath, std::string &output)
    {
        std::ifstream file(filepath);
        if (!file)
        {
            Logger::error("Failed to open file: " + filepath);
            return false;
        }

        std::stringstream buffer;
        buffer << file.rdbuf();
        output = buffer.str();

        return true;
    }