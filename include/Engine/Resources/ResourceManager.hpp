#pragma once

#include <string>
#include <unordered_map>
#include <memory>

namespace Engine
{
    class Texture;
    class Mesh;
    class Shader;
    class Material;

    /**
     * @brief Resource manager class
     *
     * The resource manager is responsible for loading and managing resources like
     * textures, meshes, shaders, and materials.
     */
    class ResourceManager
    {
    public:
        /**
         * @brief Constructor
         */
        ResourceManager();

        /**
         * @brief Destructor
         */
        ~ResourceManager();

        /**
         * @brief Initializes the resource manager
         * @return True if initialization succeeded, false otherwise
         */
        bool initialize();

        /**
         * @brief Shuts down the resource manager
         */
        void shutdown();

        /**
         * @brief Sets the resources path
         * @param path Path to the resources directory
         */
        void setResourcesPath(const std::string &path);

        /**
         * @brief Gets the full resource path
         * @param relativePath Relative path to the resource
         * @return Full path to the resource
         */
        std::string getResourcePath(const std::string &relativePath) const;

        /**
         * @brief Gets a texture by name
         * @param name Texture name
         * @return Pointer to the texture, or nullptr if not found
         */
        Texture *getTexture(const std::string &name);

        /**
         * @brief Gets a mesh by name
         * @param name Mesh name
         * @return Pointer to the mesh, or nullptr if not found
         */
        Mesh *getMesh(const std::string &name);

        /**
         * @brief Gets a shader by name
         * @param name Shader name
         * @return Pointer to the shader, or nullptr if not found
         */
        Shader *getShader(const std::string &name);

        /**
         * @brief Gets a material by name
         * @param name Material name
         * @return Pointer to the material, or nullptr if not found
         */
        Material *getMaterial(const std::string &name);

        /**
         * @brief Loads a texture from file
         * @param name Texture name
         * @param filepath Path to the texture file
         * @return Pointer to the loaded texture, or nullptr if loading failed
         */
        Texture *loadTexture(const std::string &name, const std::string &filepath);

        /**
         * @brief Loads a mesh from file
         * @param name Mesh name
         * @param filepath Path to the mesh file
         * @return Pointer to the loaded mesh, or nullptr if loading failed
         */
        Mesh *loadMesh(const std::string &name, const std::string &filepath);

        /**
         * @brief Loads a shader from files
         * @param name Shader name
         * @param vertexPath Path to the vertex shader file
         * @param fragmentPath Path to the fragment shader file
         * @return Pointer to the loaded shader, or nullptr if loading failed
         */
        Shader *loadShader(const std::string &name, const std::string &vertexPath, const std::string &fragmentPath);

        /**
         * @brief Creates a material
         * @param name Material name
         * @param shader Shader to use
         * @return Pointer to the created material, or nullptr if creation failed
         */
        Material *createMaterial(const std::string &name, Shader *shader);

    private:
        /**
         * @brief Creates a texture
         * @return Unique pointer to the created texture
         */
        std::unique_ptr<Texture> createTexture();

        /**
         * @brief Creates a mesh
         * @param name Mesh name
         * @return Unique pointer to the created mesh
         */
        std::unique_ptr<Mesh> createMesh(const std::string &name);

        /**
         * @brief Creates a shader
         * @param name Shader name
         * @return Unique pointer to the created shader
         */
        std::unique_ptr<Shader> createShader(const std::string &name);

        /**
         * @brief Loads a mesh from file
         * @param mesh Mesh to load into
         * @param filepath Path to the mesh file
         * @return True if loading succeeded, false otherwise
         */
        bool loadMeshFromFile(Mesh *mesh, const std::string &filepath);

        /**
         * @brief Loads a file to a string
         * @param filepath Path to the file
         * @param output String to load into
         * @return True if loading succeeded, false otherwise
         */
        bool loadFileToString(const std::string &filepath, std::string &output);

        /**
         * @brief Map of textures
         */
        std::unordered_map<std::string, std::unique_ptr<Texture>> textures;

        /**
         * @brief Map of meshes
         */
        std::unordered_map<std::string, std::unique_ptr<Mesh>> meshes;

        /**
         * @brief Map of shaders
         */
        std::unordered_map<std::string, std::unique_ptr<Shader>> shaders;

        /**
         * @brief Map of materials
         */
        std::unordered_map<std::string, std::unique_ptr<Material>> materials;

        /**
         * @brief Path to the resources directory
         */
        std::string resourcesPath = "resources";

        /**
         * @brief Flag indicating if the resource manager is initialized
         */
        bool initialized = false;
    };

} // namespace Engine