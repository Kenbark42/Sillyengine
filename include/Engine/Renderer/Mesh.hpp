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