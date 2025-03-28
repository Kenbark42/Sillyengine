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