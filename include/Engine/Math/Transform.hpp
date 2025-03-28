#pragma once

#include "Engine/Math/Vector.hpp"
#include "Engine/Math/Matrix.hpp"
#include "Engine/Math/Quaternion.hpp"

namespace Engine
{

    /**
     * @brief Transform class representing position, rotation, and scale
     */
    class Transform
    {
    public:
        /**
         * @brief Default constructor
         */
        Transform();

        /**
         * @brief Constructor
         * @param position Position
         * @param rotation Rotation (Euler angles in degrees)
         * @param scale Scale
         */
        Transform(const Vector3 &position, const Vector3 &rotation, const Vector3 &scale);

        /**
         * @brief Sets the position
         * @param position Position
         */
        void setPosition(const Vector3 &position);

        /**
         * @brief Sets the position
         * @param x X position
         * @param y Y position
         * @param z Z position
         */
        void setPosition(float x, float y, float z);

        /**
         * @brief Gets the position
         * @return Position
         */
        const Vector3 &getPosition() const { return position; }

        /**
         * @brief Sets the rotation (Euler angles in degrees)
         * @param rotation Rotation
         */
        void setRotation(const Vector3 &rotation);

        /**
         * @brief Sets the rotation (Euler angles in degrees)
         * @param x Rotation around X axis
         * @param y Rotation around Y axis
         * @param z Rotation around Z axis
         */
        void setRotation(float x, float y, float z);

        /**
         * @brief Gets the rotation (Euler angles in degrees)
         * @return Rotation
         */
        const Vector3 &getRotation() const { return rotation; }

        /**
         * @brief Sets the rotation from a quaternion
         * @param quaternion Quaternion
         */
        void setRotationQuaternion(const Quaternion &quaternion);

        /**
         * @brief Gets the rotation as a quaternion
         * @return Quaternion
         */
        Quaternion getRotationQuaternion() const;

        /**
         * @brief Sets the scale
         * @param scale Scale
         */
        void setScale(const Vector3 &scale);

        /**
         * @brief Sets the scale
         * @param x X scale
         * @param y Y scale
         * @param z Z scale
         */
        void setScale(float x, float y, float z);

        /**
         * @brief Sets a uniform scale
         * @param scale Scale
         */
        void setScale(float scale);

        /**
         * @brief Gets the scale
         * @return Scale
         */
        const Vector3 &getScale() const { return scale; }

        /**
         * @brief Translates the transform
         * @param translation Translation
         */
        void translate(const Vector3 &translation);

        /**
         * @brief Translates the transform
         * @param x X translation
         * @param y Y translation
         * @param z Z translation
         */
        void translate(float x, float y, float z);

        /**
         * @brief Rotates the transform (Euler angles in degrees)
         * @param rotation Rotation
         */
        void rotate(const Vector3 &rotation);

        /**
         * @brief Rotates the transform (Euler angles in degrees)
         * @param x Rotation around X axis
         * @param y Rotation around Y axis
         * @param z Rotation around Z axis
         */
        void rotate(float x, float y, float z);

        /**
         * @brief Scales the transform
         * @param scaling Scale factors
         */
        void scale(const Vector3 &scaling);

        /**
         * @brief Scales the transform
         * @param x X scale factor
         * @param y Y scale factor
         * @param z Z scale factor
         */
        void scale(float x, float y, float z);

        /**
         * @brief Applies a uniform scale
         * @param scaling Scale factor
         */
        void scale(float scaling);

        /**
         * @brief Gets the forward vector
         * @return Forward vector
         */
        Vector3 getForward() const;

        /**
         * @brief Gets the right vector
         * @return Right vector
         */
        Vector3 getRight() const;

        /**
         * @brief Gets the up vector
         * @return Up vector
         */
        Vector3 getUp() const;

        /**
         * @brief Gets the local transformation matrix
         * @return Local transformation matrix
         */
        Matrix4 getLocalMatrix() const;

        /**
         * @brief Gets the world transformation matrix
         * @return World transformation matrix
         */
        Matrix4 getWorldMatrix() const;

        /**
         * @brief Sets the parent transform
         * @param parent Parent transform
         */
        void setParent(Transform *parent);

        /**
         * @brief Gets the parent transform
         * @return Parent transform
         */
        Transform *getParent() const { return parent; }

        /**
         * @brief Resets the transform to default values
         */
        void reset();

        /**
         * @brief Interpolates between two transforms
         * @param other Target transform
         * @param t Interpolation factor (0.0 to 1.0)
         * @return Interpolated transform
         */
        Transform lerp(const Transform &other, float t) const;

    private:
        /**
         * @brief Position
         */
        Vector3 position;

        /**
         * @brief Rotation (Euler angles in degrees)
         */
        Vector3 rotation;

        /**
         * @brief Scale
         */
        Vector3 scale;

        /**
         * @brief Parent transform
         */
        Transform *parent;

        /**
         * @brief Flag indicating if the local matrix needs to be recalculated
         */
        mutable bool dirtyLocalMatrix;

        /**
         * @brief Flag indicating if the world matrix needs to be recalculated
         */
        mutable bool dirtyWorldMatrix;

        /**
         * @brief Cached local transformation matrix
         */
        mutable Matrix4 localMatrix;

        /**
         * @brief Cached world transformation matrix
         */
        mutable Matrix4 worldMatrix;
    };

} // namespace Engine