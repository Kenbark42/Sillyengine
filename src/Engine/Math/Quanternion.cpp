#include "Engine/Math/Quaternion.hpp"
#include "Engine/Math/Matrix.hpp"
#include <cmath>

namespace Engine
{

    // Constants
    const float DEG_TO_RAD = 0.01745329252f; // PI / 180
    const float RAD_TO_DEG = 57.2957795131f; // 180 / PI
    const float EPSILON = 1e-6f;

    Quaternion::Quaternion()
        : x(0.0f), y(0.0f), z(0.0f), w(1.0f)
    {
    }

    Quaternion::Quaternion(float x, float y, float z, float w)
        : x(x), y(y), z(z), w(w)
    {
    }

    Quaternion::Quaternion(const Vector3 &v, float w)
        : x(v.x), y(v.y), z(v.z), w(w)
    {
    }

    Quaternion Quaternion::operator+(const Quaternion &other) const
    {
        return Quaternion(x + other.x, y + other.y, z + other.z, w + other.w);
    }

    Quaternion Quaternion::operator-(const Quaternion &other) const
    {
        return Quaternion(x - other.x, y - other.y, z - other.z, w - other.w);
    }

    Quaternion Quaternion::operator*(const Quaternion &other) const
    {
        // Hamilton product
        return Quaternion(
            w * other.x + x * other.w + y * other.z - z * other.y,
            w * other.y - x * other.z + y * other.w + z * other.x,
            w * other.z + x * other.y - y * other.x + z * other.w,
            w * other.w - x * other.x - y * other.y - z * other.z);
    }

    Quaternion Quaternion::operator*(float scalar) const
    {
        return Quaternion(x * scalar, y * scalar, z * scalar, w * scalar);
    }

    Quaternion Quaternion::operator/(float scalar) const
    {
        float invScalar = 1.0f / scalar;
        return Quaternion(x * invScalar, y * invScalar, z * invScalar, w * invScalar);
    }

    Quaternion Quaternion::operator-() const
    {
        return Quaternion(-x, -y, -z, -w);
    }

    Quaternion &Quaternion::operator+=(const Quaternion &other)
    {
        x += other.x;
        y += other.y;
        z += other.z;
        w += other.w;
        return *this;
    }

    Quaternion &Quaternion::operator-=(const Quaternion &other)
    {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        w -= other.w;
        return *this;
    }

    Quaternion &Quaternion::operator*=(const Quaternion &other)
    {
        // Hamilton product
        float newX = w * other.x + x * other.w + y * other.z - z * other.y;
        float newY = w * other.y - x * other.z + y * other.w + z * other.x;
        float newZ = w * other.z + x * other.y - y * other.x + z * other.w;
        float newW = w * other.w - x * other.x - y * other.y - z * other.z;

        x = newX;
        y = newY;
        z = newZ;
        w = newW;

        return *this;
    }

    Quaternion &Quaternion::operator*=(float scalar)
    {
        x *= scalar;
        y *= scalar;
        z *= scalar;
        w *= scalar;
        return *this;
    }

    Quaternion &Quaternion::operator/=(float scalar)
    {
        float invScalar = 1.0f / scalar;
        x *= invScalar;
        y *= invScalar;
        z *= invScalar;
        w *= invScalar;
        return *this;
    }

    bool Quaternion::operator==(const Quaternion &other) const
    {
        return (std::abs(x - other.x) < EPSILON &&
                std::abs(y - other.y) < EPSILON &&
                std::abs(z - other.z) < EPSILON &&
                std::abs(w - other.w) < EPSILON);
    }

    bool Quaternion::operator!=(const Quaternion &other) const
    {
        return !(*this == other);
    }

    float Quaternion::lengthSquared() const
    {
        return x * x + y * y + z * z + w * w;
    }

    float Quaternion::length() const
    {
        return std::sqrt(lengthSquared());
    }

    Quaternion Quaternion::normalized() const
    {
        float len = length();
        if (len < EPSILON)
        {
            return Quaternion(0.0f, 0.0f, 0.0f, 1.0f);
        }

        float invLen = 1.0f / len;
        return Quaternion(x * invLen, y * invLen, z * invLen, w * invLen);
    }

    Quaternion &Quaternion::normalize()
    {
        float len = length();
        if (len < EPSILON)
        {
            x = 0.0f;
            y = 0.0f;
            z = 0.0f;
            w = 1.0f;
            return *this;
        }

        float invLen = 1.0f / len;
        x *= invLen;
        y *= invLen;
        z *= invLen;
        w *= invLen;

        return *this;
    }

    Quaternion Quaternion::conjugate() const
    {
        return Quaternion(-x, -y, -z, w);
    }

    Quaternion Quaternion::inverse() const
    {
        float lenSq = lengthSquared();
        if (lenSq < EPSILON)
        {
            return Quaternion(0.0f, 0.0f, 0.0f, 0.0f);
        }

        float invLenSq = 1.0f / lenSq;
        return Quaternion(-x * invLenSq, -y * invLenSq, -z * invLenSq, w * invLenSq);
    }

    float Quaternion::dot(const Quaternion &other) const
    {
        return x * other.x + y * other.y + z * other.z + w * other.w;
    }

    Vector3 Quaternion::rotate(const Vector3 &v) const
    {
        // Convert vector to quaternion (with w = 0)
        Quaternion vecQuat(v.x, v.y, v.z, 0.0f);

        // Calculate q * v * q^-1
        Quaternion result = *this * vecQuat * this->conjugate();

        // Extract vector part
        return Vector3(result.x, result.y, result.z);
    }

    Vector3 Quaternion::toEulerAngles() const
    {
        // Normalized quaternion
        Quaternion q = this->normalized();

        // Roll (x-axis rotation)
        float sinr_cosp = 2.0f * (q.w * q.x + q.y * q.z);
        float cosr_cosp = 1.0f - 2.0f * (q.x * q.x + q.y * q.y);
        float roll = std::atan2(sinr_cosp, cosr_cosp);

        // Pitch (y-axis rotation)
        float sinp = 2.0f * (q.w * q.y - q.z * q.x);
        float pitch;
        if (std::abs(sinp) >= 1.0f)
        {
            // Use 90 degrees if out of range
            pitch = std::copysign(M_PI / 2.0f, sinp);
        }
        else
        {
            pitch = std::asin(sinp);
        }

        // Yaw (z-axis rotation)
        float siny_cosp = 2.0f * (q.w * q.z + q.x * q.y);
        float cosy_cosp = 1.0f - 2.0f * (q.y * q.y + q.z * q.z);
        float yaw = std::atan2(siny_cosp, cosy_cosp);

        return Vector3(roll, pitch, yaw);
    }

    Vector3 Quaternion::toEulerAnglesDegrees() const
    {
        Vector3 radians = toEulerAngles();
        return Vector3(
            radians.x * RAD_TO_DEG,
            radians.y * RAD_TO_DEG,
            radians.z * RAD_TO_DEG);
    }

    Matrix3 Quaternion::toMatrix3() const
    {
        // Normalize quaternion
        Quaternion q = this->normalized();

        // Calculate quaternion components squared
        float x2 = q.x * q.x;
        float y2 = q.y * q.y;
        float z2 = q.z * q.z;
        float w2 = q.w * q.w;

        // Calculate 2*products
        float xy = 2.0f * q.x * q.y;
        float xz = 2.0f * q.x * q.z;
        float xw = 2.0f * q.x * q.w;
        float yz = 2.0f * q.y * q.z;
        float yw = 2.0f * q.y * q.w;
        float zw = 2.0f * q.z * q.w;

        // Create rotation matrix
        return Matrix3(
            w2 + x2 - y2 - z2, xy - zw, xz + yw,
            xy + zw, w2 - x2 + y2 - z2, yz - xw,
            xz - yw, yz + xw, w2 - x2 - y2 + z2);
    }

    Matrix4 Quaternion::toMatrix4() const
    {
        Matrix3 rot = toMatrix3();
        return Matrix4(rot);
    }

    Quaternion Quaternion::identity()
    {
        return Quaternion(0.0f, 0.0f, 0.0f, 1.0f);
    }

    Quaternion Quaternion::fromEulerAngles(const Vector3 &eulerAngles)
    {
        return fromEulerAngles(eulerAngles.x, eulerAngles.y, eulerAngles.z);
    }

    Quaternion Quaternion::fromEulerAngles(float x, float y, float z)
    {
        // Calculate half angles
        float halfX = x * 0.5f;
        float halfY = y * 0.5f;
        float halfZ = z * 0.5f;

        // Calculate sines and cosines
        float cx = std::cos(halfX);
        float sx = std::sin(halfX);
        float cy = std::cos(halfY);
        float sy = std::sin(halfY);
        float cz = std::cos(halfZ);
        float sz = std::sin(halfZ);

        // Calculate quaternion components
        return Quaternion(
            sx * cy * cz - cx * sy * sz,
            cx * sy * cz + sx * cy * sz,
            cx * cy * sz - sx * sy * cz,
            cx * cy * cz + sx * sy * sz);
    }

    Quaternion Quaternion::fromEulerAnglesDegrees(const Vector3 &eulerAngles)
    {
        return fromEulerAnglesDegrees(eulerAngles.x, eulerAngles.y, eulerAngles.z);
    }

    Quaternion Quaternion::fromEulerAnglesDegrees(float x, float y, float z)
    {
        // Convert degrees to radians
        return fromEulerAngles(
            x * DEG_TO_RAD,
            y * DEG_TO_RAD,
            z * DEG_TO_RAD);
    }

    Quaternion Quaternion::fromAxisAngle(const Vector3 &axis, float angle)
    {
        // Half angle
        float halfAngle = angle * 0.5f;

        // Calculate sine and cosine
        float sinHalfAngle = std::sin(halfAngle);
        float cosHalfAngle = std::cos(halfAngle);

        // Normalize axis
        Vector3 normalizedAxis = axis.normalized();

        // Create quaternion
        return Quaternion(
            normalizedAxis.x * sinHalfAngle,
            normalizedAxis.y * sinHalfAngle,
            normalizedAxis.z * sinHalfAngle,
            cosHalfAngle);
    }

    Quaternion Quaternion::fromToRotation(const Vector3 &from, const Vector3 &to)
    {
        // Normalize input vectors
        Vector3 fromNorm = from.normalized();
        Vector3 toNorm = to.normalized();

        // Check if vectors are same or opposite
        float dot = fromNorm.dot(toNorm);

        // If vectors are nearly the same, return identity
        if (dot > 0.999999f)
        {
            return Quaternion::identity();
        }

        // If vectors are nearly opposite, find a perpendicular vector
        if (dot < -0.999999f)
        {
            // Find axis perpendicular to from
            Vector3 axis = Vector3(1.0f, 0.0f, 0.0f).cross(fromNorm);

            // If axis is nearly zero, try another axis
            if (axis.lengthSquared() < 0.000001f)
            {
                axis = Vector3(0.0f, 1.0f, 0.0f).cross(fromNorm);
            }

            // Return rotation of 180 degrees around the perpendicular axis
            return Quaternion::fromAxisAngle(axis.normalized(), M_PI);
        }

        // Find axis of rotation: the cross product of from and to
        Vector3 axis = fromNorm.cross(toNorm).normalized();

        // Find angle of rotation: the angle between from and to
        float angle = std::acos(dot);

        // Return rotation
        return Quaternion::fromAxisAngle(axis, angle);
    }

    Quaternion Quaternion::lookRotation(const Vector3 &forward, const Vector3 &up)
    {
        // Forward and up must be normalized
        Vector3 forwardNorm = forward.normalized();
        Vector3 upNorm = up.normalized();

        // Handle case where forward and up are parallel
        float dot = std::abs(forwardNorm.dot(upNorm));
        if (dot > 0.999999f)
        {
            // Choose a different up vector
            upNorm = (std::abs(forwardNorm.dot(Vector3::Up)) > 0.999999f) ? Vector3::Right : Vector3::Up;
        }

        // Calculate coordinate axes
        Vector3 right = upNorm.cross(forwardNorm).normalized();
        Vector3 orthoUp = forwardNorm.cross(right).normalized();

        // Create rotation matrix
        Matrix3 rotMatrix(
            right.x, right.y, right.z,
            orthoUp.x, orthoUp.y, orthoUp.z,
            forwardNorm.x, forwardNorm.y, forwardNorm.z);

        // Extract quaternion from rotation matrix
        float trace = rotMatrix.get(0, 0) + rotMatrix.get(1, 1) + rotMatrix.get(2, 2);

        if (trace > 0.0f)
        {
            float s = 0.5f / std::sqrt(trace + 1.0f);
            return Quaternion(
                (rotMatrix.get(2, 1) - rotMatrix.get(1, 2)) * s,
                (rotMatrix.get(0, 2) - rotMatrix.get(2, 0)) * s,
                (rotMatrix.get(1, 0) - rotMatrix.get(0, 1)) * s,
                0.25f / s);
        }
        else
        {
            if (rotMatrix.get(0, 0) > rotMatrix.get(1, 1) && rotMatrix.get(0, 0) > rotMatrix.get(2, 2))
            {
                float s = 2.0f * std::sqrt(1.0f + rotMatrix.get(0, 0) - rotMatrix.get(1, 1) - rotMatrix.get(2, 2));
                return Quaternion(
                    0.25f * s,
                    (rotMatrix.get(0, 1) + rotMatrix.get(1, 0)) / s,
                    (rotMatrix.get(0, 2) + rotMatrix.get(2, 0)) / s,
                    (rotMatrix.get(2, 1) - rotMatrix.get(1, 2)) / s);
            }
            else if (rotMatrix.get(1, 1) > rotMatrix.get(2, 2))
            {
                float s = 2.0f * std::sqrt(1.0f + rotMatrix.get(1, 1) - rotMatrix.get(0, 0) - rotMatrix.get(2, 2));
                return Quaternion(
                    (rotMatrix.get(0, 1) + rotMatrix.get(1, 0)) / s,
                    0.25f * s,
                    (rotMatrix.get(1, 2) + rotMatrix.get(2, 1)) / s,
                    (rotMatrix.get(0, 2) - rotMatrix.get(2, 0)) / s);
            }
            else
            {
                float s = 2.0f * std::sqrt(1.0f + rotMatrix.get(2, 2) - rotMatrix.get(0, 0) - rotMatrix.get(1, 1));
                return Quaternion(
                    (rotMatrix.get(0, 2) + rotMatrix.get(2, 0)) / s,
                    (rotMatrix.get(1, 2) + rotMatrix.get(2, 1)) / s,
                    0.25f * s,
                    (rotMatrix.get(1, 0) - rotMatrix.get(0, 1)) / s);
            }
        }
    }

    Quaternion Quaternion::slerp(const Quaternion &a, const Quaternion &b, float t)
    {
        // Clamp t between 0 and 1
        t = (t < 0.0f) ? 0.0f : ((t > 1.0f) ? 1.0f : t);

        // Normalized quaternions
        Quaternion qa = a.normalized();
        Quaternion qb = b.normalized();

        // Calculate dot product
        float dot = qa.dot(qb);

        // If dot is negative, invert one quaternion to take the shortest path
        if (dot < 0.0f)
        {
            qb = -qb;
            dot = -dot;
        }

        // If quaternions are very close, use linear interpolation
        if (dot > 0.9995f)
        {
            return Quaternion::lerp(qa, qb, t).normalized();
        }

        // Calculate angle and sin
        float angle = std::acos(dot);
        float sinAngle = std::sin(angle);

        // Calculate factors
        float factorA = std::sin((1.0f - t) * angle) / sinAngle;
        float factorB = std::sin(t * angle) / sinAngle;

        // Interpolate
        return Quaternion(
            qa.x * factorA + qb.x * factorB,
            qa.y * factorA + qb.y * factorB,
            qa.z * factorA + qb.z * factorB,
            qa.w * factorA + qb.w * factorB);
    }

    Quaternion Quaternion::lerp(const Quaternion &a, const Quaternion &b, float t)
    {
        // Clamp t between 0 and 1
        t = (t < 0.0f) ? 0.0f : ((t > 1.0f) ? 1.0f : t);

        // Linear interpolation
        Quaternion result(
            a.x * (1.0f - t) + b.x * t,
            a.y * (1.0f - t) + b.y * t,
            a.z * (1.0f - t) + b.z * t,
            a.w * (1.0f - t) + b.w * t);

        // Normalize result
        return result.normalized();
    }

} // namespace Engine