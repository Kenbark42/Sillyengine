#pragma once

#include "Engine/Math/Vector.hpp"

namespace Engine
{

    class Matrix3;
    class Matrix4;

    /**
     * @brief Quaternion class for representing rotations
     */
    class Quaternion
    {
    public:
        /**
         * @brief Default constructor (identity quaternion)
         */
        Quaternion();

        /**
         * @brief Constructor
         * @param x X component
         * @param y Y component
         * @param z Z component
         * @param w W component
         */
        Quaternion(float x, float y, float z, float w);

        /**
         * @brief Constructor from a vector and scalar
         * @param v Vector part (x, y, z)
         * @param w Scalar part
         */
        Quaternion(const Vector3 &v, float w);

        /**
         * @brief Gets the x component
         * @return X component
         */
        float getX() const { return x; }

        /**
         * @brief Gets the y component
         * @return Y component
         */
        float getY() const { return y; }

        /**
         * @brief Gets the z component
         * @return Z component
         */
        float getZ() const { return z; }

        /**
         * @brief Gets the w component
         * @return W component
         */
        float getW() const { return w; }

        /**
         * @brief Sets the x component
         * @param x X component
         */
        void setX(float x) { this->x = x; }

        /**
         * @brief Sets the y component
         * @param y Y component
         */
        void setY(float y) { this->y = y; }

        /**
         * @brief Sets the z component
         * @param z Z component
         */
        void setZ(float z) { this->z = z; }

        /**
         * @brief Sets the w component
         * @param w W component
         */
        void setW(float w) { this->w = w; }

        /**
         * @brief Gets the vector part (x, y, z)
         * @return Vector part
         */
        Vector3 getVector() const { return Vector3(x, y, z); }

        /**
         * @brief Gets the scalar part (w)
         * @return Scalar part
         */
        float getScalar() const { return w; }

        /**
         * @brief Addition operator
         * @param other Quaternion to add
         * @return Result of the addition
         */
        Quaternion operator+(const Quaternion &other) const;

        /**
         * @brief Subtraction operator
         * @param other Quaternion to subtract
         * @return Result of the subtraction
         */
        Quaternion operator-(const Quaternion &other) const;

        /**
         * @brief Multiplication operator (quaternion-quaternion)
         * @param other Quaternion to multiply by
         * @return Result of the multiplication
         */
        Quaternion operator*(const Quaternion &other) const;

        /**
         * @brief Multiplication operator (quaternion-scalar)
         * @param scalar Scalar to multiply by
         * @return Result of the multiplication
         */
        Quaternion operator*(float scalar) const;

        /**
         * @brief Division operator (quaternion-scalar)
         * @param scalar Scalar to divide by
         * @return Result of the division
         */
        Quaternion operator/(float scalar) const;

        /**
         * @brief Negation operator
         * @return Negated quaternion
         */
        Quaternion operator-() const;

        /**
         * @brief Addition assignment operator
         * @param other Quaternion to add
         * @return Reference to this quaternion
         */
        Quaternion &operator+=(const Quaternion &other);

        /**
         * @brief Subtraction assignment operator
         * @param other Quaternion to subtract
         * @return Reference to this quaternion
         */
        Quaternion &operator-=(const Quaternion &other);

        /**
         * @brief Multiplication assignment operator (quaternion-quaternion)
         * @param other Quaternion to multiply by
         * @return Reference to this quaternion
         */
        Quaternion &operator*=(const Quaternion &other);

        /**
         * @brief Multiplication assignment operator (quaternion-scalar)
         * @param scalar Scalar to multiply by
         * @return Reference to this quaternion
         */
        Quaternion &operator*=(float scalar);

        /**
         * @brief Division assignment operator (quaternion-scalar)
         * @param scalar Scalar to divide by
         * @return Reference to this quaternion
         */
        Quaternion &operator/=(float scalar);

        /**
         * @brief Equality operator
         * @param other Quaternion to compare with
         * @return True if the quaternions are equal, false otherwise
         */
        bool operator==(const Quaternion &other) const;

        /**
         * @brief Inequality operator
         * @param other Quaternion to compare with
         * @return True if the quaternions are not equal, false otherwise
         */
        bool operator!=(const Quaternion &other) const;

        /**
         * @brief Calculates the length squared
         * @return Length squared
         */
        float lengthSquared() const;

        /**
         * @brief Calculates the length
         * @return Length
         */
        float length() const;

        /**
         * @brief Normalizes the quaternion
         * @return Normalized quaternion
         */
        Quaternion normalized() const;

        /**
         * @brief Normalizes the quaternion in-place
         * @return Reference to this quaternion
         */
        Quaternion &normalize();

        /**
         * @brief Calculates the conjugate of the quaternion
         * @return Conjugate
         */
        Quaternion conjugate() const;

        /**
         * @brief Calculates the inverse of the quaternion
         * @return Inverse
         */
        Quaternion inverse() const;

        /**
         * @brief Calculates the dot product
         * @param other Quaternion to calculate the dot product with
         * @return Dot product
         */
        float dot(const Quaternion &other) const;

        /**
         * @brief Rotates a vector by this quaternion
         * @param v Vector to rotate
         * @return Rotated vector
         */
        Vector3 rotate(const Vector3 &v) const;

        /**
         * @brief Converts the quaternion to Euler angles (in radians)
         * @return Euler angles (in radians)
         */
        Vector3 toEulerAngles() const;

        /**
         * @brief Converts the quaternion to Euler angles (in degrees)
         * @return Euler angles (in degrees)
         */
        Vector3 toEulerAnglesDegrees() const;

        /**
         * @brief Converts the quaternion to a 3x3 matrix
         * @return 3x3 rotation matrix
         */
        Matrix3 toMatrix3() const;

        /**
         * @brief Converts the quaternion to a 4x4 matrix
         * @return 4x4 rotation matrix
         */
        Matrix4 toMatrix4() const;

        /**
         * @brief Creates an identity quaternion
         * @return Identity quaternion
         */
        static Quaternion identity();

        /**
         * @brief Creates a quaternion from Euler angles (in radians)
         * @param eulerAngles Euler angles (in radians)
         * @return Quaternion
         */
        static Quaternion fromEulerAngles(const Vector3 &eulerAngles);

        /**
         * @brief Creates a quaternion from Euler angles (in radians)
         * @param x Rotation around X axis
         * @param y Rotation around Y axis
         * @param z Rotation around Z axis
         * @return Quaternion
         */
        static Quaternion fromEulerAngles(float x, float y, float z);

        /**
         * @brief Creates a quaternion from Euler angles (in degrees)
         * @param eulerAngles Euler angles (in degrees)
         * @return Quaternion
         */
        static Quaternion fromEulerAnglesDegrees(const Vector3 &eulerAngles);

        /**
         * @brief Creates a quaternion from Euler angles (in degrees)
         * @param x Rotation around X axis
         * @param y Rotation around Y axis
         * @param z Rotation around Z axis
         * @return Quaternion
         */
        static Quaternion fromEulerAnglesDegrees(float x, float y, float z);

        /**
         * @brief Creates a quaternion from an axis and angle
         * @param axis Rotation axis (normalized)
         * @param angle Rotation angle (in radians)
         * @return Quaternion
         */
        static Quaternion fromAxisAngle(const Vector3 &axis, float angle);

        /**
         * @brief Creates a quaternion that rotates from one vector to another
         * @param from Source vector
         * @param to Target vector
         * @return Quaternion
         */
        static Quaternion fromToRotation(const Vector3 &from, const Vector3 &to);

        /**
         * @brief Creates a quaternion that looks in a specific direction
         * @param forward Forward direction
         * @param up Up direction
         * @return Quaternion
         */
        static Quaternion lookRotation(const Vector3 &forward, const Vector3 &up = Vector3::Up);

        /**
         * @brief Interpolates between two quaternions (SLERP)
         * @param a First quaternion
         * @param b Second quaternion
         * @param t Interpolation factor (0.0 to 1.0)
         * @return Interpolated quaternion
         */
        static Quaternion slerp(const Quaternion &a, const Quaternion &b, float t);

        /**
         * @brief Linearly interpolates between two quaternions (NLERP)
         * @param a First quaternion
         * @param b Second quaternion
         * @param t Interpolation factor (0.0 to 1.0)
         * @return Interpolated quaternion
         */
        static Quaternion lerp(const Quaternion &a, const Quaternion &b, float t);

    private:
        /**
         * @brief X component
         */
        float x;

        /**
         * @brief Y component
         */
        float y;

        /**
         * @brief Z component
         */
        float z;

        /**
         * @brief W component
         */
        float w;
    };

} // namespace Engine