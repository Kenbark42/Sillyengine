#include "Engine/Math/Transform.hpp"
#include <cmath>

namespace Engine
{

    Transform::Transform()
        : position(Vector3::Zero),
          rotation(Vector3::Zero),
          scale(Vector3::One),
          parent(nullptr),
          dirtyLocalMatrix(true),
          dirtyWorldMatrix(true)
    {
    }

    Transform::Transform(const Vector3 &position, const Vector3 &rotation, const Vector3 &scale)
        : position(position),
          rotation(rotation),
          scale(scale),
          parent(nullptr),
          dirtyLocalMatrix(true),
          dirtyWorldMatrix(true)
    {
    }

    void Transform::setPosition(const Vector3 &position)
    {
        this->position = position;
        dirtyLocalMatrix = true;
        dirtyWorldMatrix = true;
    }

    void Transform::setPosition(float x, float y, float z)
    {
        setPosition(Vector3(x, y, z));
    }

    void Transform::setRotation(const Vector3 &rotation)
    {
        this->rotation = rotation;
        dirtyLocalMatrix = true;
        dirtyWorldMatrix = true;
    }

    void Transform::setRotation(float x, float y, float z)
    {
        setRotation(Vector3(x, y, z));
    }

    void Transform::setRotationQuaternion(const Quaternion &quaternion)
    {
        // Convert quaternion to Euler angles (in degrees)
        rotation = quaternion.toEulerAnglesDegrees();
        dirtyLocalMatrix = true;
        dirtyWorldMatrix = true;
    }

    Quaternion Transform::getRotationQuaternion() const
    {
        // Convert Euler angles (in degrees) to quaternion
        return Quaternion::fromEulerAnglesDegrees(rotation);
    }

    void Transform::setScale(const Vector3 &scale)
    {
        this->scale = scale;
        dirtyLocalMatrix = true;
        dirtyWorldMatrix = true;
    }

    void Transform::setScale(float x, float y, float z)
    {
        setScale(Vector3(x, y, z));
    }

    void Transform::setScale(float scale)
    {
        setScale(Vector3(scale));
    }

    void Transform::translate(const Vector3 &translation)
    {
        position += translation;
        dirtyLocalMatrix = true;
        dirtyWorldMatrix = true;
    }

    void Transform::translate(float x, float y, float z)
    {
        translate(Vector3(x, y, z));
    }

    void Transform::rotate(const Vector3 &rotation)
    {
        this->rotation += rotation;
        dirtyLocalMatrix = true;
        dirtyWorldMatrix = true;
    }

    void Transform::rotate(float x, float y, float z)
    {
        rotate(Vector3(x, y, z));
    }

    void Transform::scale(const Vector3 &scaling)
    {
        this->scale = Vector3(
            this->scale.x * scaling.x,
            this->scale.y * scaling.y,
            this->scale.z * scaling.z);
        dirtyLocalMatrix = true;
        dirtyWorldMatrix = true;
    }

    void Transform::scale(float x, float y, float z)
    {
        scale(Vector3(x, y, z));
    }

    void Transform::scale(float scaling)
    {
        scale(Vector3(scaling));
    }

    Vector3 Transform::getForward() const
    {
        // Get the rotation quaternion
        Quaternion q = getRotationQuaternion();

        // Rotate the forward vector (0, 0, 1) by the quaternion
        return q.rotate(Vector3::Forward);
    }

    Vector3 Transform::getRight() const
    {
        // Get the rotation quaternion
        Quaternion q = getRotationQuaternion();

        // Rotate the right vector (1, 0, 0) by the quaternion
        return q.rotate(Vector3::Right);
    }

    Vector3 Transform::getUp() const
    {
        // Get the rotation quaternion
        Quaternion q = getRotationQuaternion();

        // Rotate the up vector (0, 1, 0) by the quaternion
        return q.rotate(Vector3::Up);
    }

    Matrix4 Transform::getLocalMatrix() const
    {
        if (dirtyLocalMatrix)
        {
            // Create translation matrix
            Matrix4 translationMatrix = Matrix4::translation(position);

            // Create rotation matrix (from Euler angles in degrees)
            Matrix4 rotationMatrix = Matrix4::rotation(
                rotation.x * 0.01745329252f, // Convert degrees to radians (pi/180)
                rotation.y * 0.01745329252f,
                rotation.z * 0.01745329252f);

            // Create scale matrix
            Matrix4 scaleMatrix = Matrix4::scaling(scale);

            // Combine matrices: translation * rotation * scale
            localMatrix = translationMatrix * rotationMatrix * scaleMatrix;

            dirtyLocalMatrix = false;
        }

        return localMatrix;
    }

    Matrix4 Transform::getWorldMatrix() const
    {
        if (dirtyWorldMatrix)
        {
            if (parent)
            {
                // Combine with parent matrix if there is a parent
                worldMatrix = parent->getWorldMatrix() * getLocalMatrix();
            }
            else
            {
                // Just use local matrix if there is no parent
                worldMatrix = getLocalMatrix();
            }

            dirtyWorldMatrix = false;
        }

        return worldMatrix;
    }

    void Transform::setParent(Transform *parent)
    {
        // Skip if parent is already set
        if (this->parent == parent)
        {
            return;
        }

        this->parent = parent;
        dirtyWorldMatrix = true;
    }

    void Transform::reset()
    {
        position = Vector3::Zero;
        rotation = Vector3::Zero;
        scale = Vector3::One;
        parent = nullptr;
        dirtyLocalMatrix = true;
        dirtyWorldMatrix = true;
    }

    Transform Transform::lerp(const Transform &other, float t) const
    {
        // Clamp t between 0 and 1
        t = (t < 0.0f) ? 0.0f : ((t > 1.0f) ? 1.0f : t);

        // Interpolate position
        Vector3 newPosition = position.lerp(other.position, t);

        // Interpolate rotation using quaternions for smoother interpolation
        Quaternion q1 = getRotationQuaternion();
        Quaternion q2 = other.getRotationQuaternion();
        Quaternion newRotationQuat = Quaternion::slerp(q1, q2, t);
        Vector3 newRotation = newRotationQuat.toEulerAnglesDegrees();

        // Interpolate scale
        Vector3 newScale = scale.lerp(other.scale, t);

        return Transform(newPosition, newRotation, newScale);
    }

} // namespace Engine