#include "Engine/Math/Matrix.hpp"
#include "Engine/Math/Quaternion.hpp"
#include <cmath>

namespace Engine
{

    // Constants
    const float DEG_TO_RAD = 0.01745329252f; // PI / 180
    const float EPSILON = 1e-6f;

    //-----------------------------------------------------------------------------------
    // Matrix3 Implementation
    //-----------------------------------------------------------------------------------

    Matrix3::Matrix3()
    {
        // Initialize as identity matrix
        data = {
            1.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 1.0f};
    }

    Matrix3::Matrix3(const std::array<float, 9> &data)
        : data(data)
    {
    }

    Matrix3::Matrix3(float m00, float m01, float m02,
                     float m10, float m11, float m12,
                     float m20, float m21, float m22)
    {
        data = {
            m00, m01, m02,
            m10, m11, m12,
            m20, m21, m22};
    }

    float Matrix3::get(int row, int col) const
    {
        return data[row * 3 + col];
    }

    void Matrix3::set(int row, int col, float value)
    {
        data[row * 3 + col] = value;
    }

    Vector3 Matrix3::getRow(int row) const
    {
        int index = row * 3;
        return Vector3(data[index], data[index + 1], data[index + 2]);
    }

    Vector3 Matrix3::getColumn(int col) const
    {
        return Vector3(data[col], data[col + 3], data[col + 6]);
    }

    Matrix3 Matrix3::operator+(const Matrix3 &other) const
    {
        std::array<float, 9> result;
        for (int i = 0; i < 9; ++i)
        {
            result[i] = data[i] + other.data[i];
        }
        return Matrix3(result);
    }

    Matrix3 Matrix3::operator-(const Matrix3 &other) const
    {
        std::array<float, 9> result;
        for (int i = 0; i < 9; ++i)
        {
            result[i] = data[i] - other.data[i];
        }
        return Matrix3(result);
    }

    Matrix3 Matrix3::operator*(const Matrix3 &other) const
    {
        Matrix3 result;

        for (int row = 0; row < 3; ++row)
        {
            for (int col = 0; col < 3; ++col)
            {
                float sum = 0.0f;
                for (int i = 0; i < 3; ++i)
                {
                    sum += get(row, i) * other.get(i, col);
                }
                result.set(row, col, sum);
            }
        }

        return result;
    }

    Vector3 Matrix3::operator*(const Vector3 &vec) const
    {
        return Vector3(
            get(0, 0) * vec.x + get(0, 1) * vec.y + get(0, 2) * vec.z,
            get(1, 0) * vec.x + get(1, 1) * vec.y + get(1, 2) * vec.z,
            get(2, 0) * vec.x + get(2, 1) * vec.y + get(2, 2) * vec.z);
    }

    Matrix3 Matrix3::operator*(float scalar) const
    {
        std::array<float, 9> result;
        for (int i = 0; i < 9; ++i)
        {
            result[i] = data[i] * scalar;
        }
        return Matrix3(result);
    }

    Matrix3 Matrix3::operator/(float scalar) const
    {
        float invScalar = 1.0f / scalar;
        return (*this) * invScalar;
    }

    Matrix3 &Matrix3::operator+=(const Matrix3 &other)
    {
        for (int i = 0; i < 9; ++i)
        {
            data[i] += other.data[i];
        }
        return *this;
    }

    Matrix3 &Matrix3::operator-=(const Matrix3 &other)
    {
        for (int i = 0; i < 9; ++i)
        {
            data[i] -= other.data[i];
        }
        return *this;
    }

    Matrix3 &Matrix3::operator*=(const Matrix3 &other)
    {
        *this = (*this) * other;
        return *this;
    }

    Matrix3 &Matrix3::operator*=(float scalar)
    {
        for (int i = 0; i < 9; ++i)
        {
            data[i] *= scalar;
        }
        return *this;
    }

    Matrix3 &Matrix3::operator/=(float scalar)
    {
        float invScalar = 1.0f / scalar;
        return (*this) *= invScalar;
    }

    bool Matrix3::operator==(const Matrix3 &other) const
    {
        for (int i = 0; i < 9; ++i)
        {
            if (std::abs(data[i] - other.data[i]) > EPSILON)
            {
                return false;
            }
        }
        return true;
    }

    bool Matrix3::operator!=(const Matrix3 &other) const
    {
        return !(*this == other);
    }

    float Matrix3::determinant() const
    {
        return get(0, 0) * (get(1, 1) * get(2, 2) - get(1, 2) * get(2, 1)) -
               get(0, 1) * (get(1, 0) * get(2, 2) - get(1, 2) * get(2, 0)) +
               get(0, 2) * (get(1, 0) * get(2, 1) - get(1, 1) * get(2, 0));
    }

    Matrix3 Matrix3::inverse() const
    {
        // Calculate determinant
        float det = determinant();

        // Check if matrix is invertible
        if (std::abs(det) < EPSILON)
        {
            return Matrix3::identity(); // Return identity if not invertible
        }

        float invDet = 1.0f / det;

        Matrix3 result;

        // Calculate adjugate (transpose of cofactor matrix) and multiply by 1/det
        result.set(0, 0, (get(1, 1) * get(2, 2) - get(1, 2) * get(2, 1)) * invDet);
        result.set(0, 1, (get(0, 2) * get(2, 1) - get(0, 1) * get(2, 2)) * invDet);
        result.set(0, 2, (get(0, 1) * get(1, 2) - get(0, 2) * get(1, 1)) * invDet);

        result.set(1, 0, (get(1, 2) * get(2, 0) - get(1, 0) * get(2, 2)) * invDet);
        result.set(1, 1, (get(0, 0) * get(2, 2) - get(0, 2) * get(2, 0)) * invDet);
        result.set(1, 2, (get(0, 2) * get(1, 0) - get(0, 0) * get(1, 2)) * invDet);

        result.set(2, 0, (get(1, 0) * get(2, 1) - get(1, 1) * get(2, 0)) * invDet);
        result.set(2, 1, (get(0, 1) * get(2, 0) - get(0, 0) * get(2, 1)) * invDet);
        result.set(2, 2, (get(0, 0) * get(1, 1) - get(0, 1) * get(1, 0)) * invDet);

        return result;
    }

    Matrix3 Matrix3::transpose() const
    {
        Matrix3 result;

        for (int row = 0; row < 3; ++row)
        {
            for (int col = 0; col < 3; ++col)
            {
                result.set(col, row, get(row, col));
            }
        }

        return result;
    }

    Matrix3 Matrix3::identity()
    {
        return Matrix3(
            1.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 1.0f);
    }

    Matrix3 Matrix3::scaling(const Vector3 &scale)
    {
        return scaling(scale.x, scale.y, scale.z);
    }

    Matrix3 Matrix3::scaling(float sx, float sy, float sz)
    {
        return Matrix3(
            sx, 0.0f, 0.0f,
            0.0f, sy, 0.0f,
            0.0f, 0.0f, sz);
    }

    Matrix3 Matrix3::scaling(float scale)
    {
        return scaling(scale, scale, scale);
    }

    Matrix3 Matrix3::rotationX(float angle)
    {
        float c = std::cos(angle);
        float s = std::sin(angle);

        return Matrix3(
            1.0f, 0.0f, 0.0f,
            0.0f, c, -s,
            0.0f, s, c);
    }

    Matrix3 Matrix3::rotationY(float angle)
    {
        float c = std::cos(angle);
        float s = std::sin(angle);

        return Matrix3(
            c, 0.0f, s,
            0.0f, 1.0f, 0.0f,
            -s, 0.0f, c);
    }

    Matrix3 Matrix3::rotationZ(float angle)
    {
        float c = std::cos(angle);
        float s = std::sin(angle);

        return Matrix3(
            c, -s, 0.0f,
            s, c, 0.0f,
            0.0f, 0.0f, 1.0f);
    }

    Matrix3 Matrix3::rotation(float x, float y, float z)
    {
        // Convert Euler angles to rotation matrices
        Matrix3 rx = rotationX(x);
        Matrix3 ry = rotationY(y);
        Matrix3 rz = rotationZ(z);

        // Combine rotations: Z * Y * X
        return rz * ry * rx;
    }

    Matrix3 Matrix3::rotation(const Quaternion &q)
    {
        return q.toMatrix3();
    }

    //-----------------------------------------------------------------------------------
    // Matrix4 Implementation
    //-----------------------------------------------------------------------------------

    Matrix4::Matrix4()
    {
        // Initialize as identity matrix
        data = {
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f};
    }

    Matrix4::Matrix4(const std::array<float, 16> &data)
        : data(data)
    {
    }

    Matrix4::Matrix4(float m00, float m01, float m02, float m03,
                     float m10, float m11, float m12, float m13,
                     float m20, float m21, float m22, float m23,
                     float m30, float m31, float m32, float m33)
    {
        data = {
            m00, m01, m02, m03,
            m10, m11, m12, m13,
            m20, m21, m22, m23,
            m30, m31, m32, m33};
    }

    Matrix4::Matrix4(const Matrix3 &m)
    {
        // Copy 3x3 matrix to upper-left corner and set the rest to identity
        for (int row = 0; row < 3; ++row)
        {
            for (int col = 0; col < 3; ++col)
            {
                set(row, col, m.get(row, col));
            }
            set(row, 3, 0.0f);
        }

        set(3, 0, 0.0f);
        set(3, 1, 0.0f);
        set(3, 2, 0.0f);
        set(3, 3, 1.0f);
    }

    float Matrix4::get(int row, int col) const
    {
        return data[row * 4 + col];
    }

    void Matrix4::set(int row, int col, float value)
    {
        data[row * 4 + col] = value;
    }

    Vector4 Matrix4::getRow(int row) const
    {
        int index = row * 4;
        return Vector4(data[index], data[index + 1], data[index + 2], data[index + 3]);
    }

    Vector4 Matrix4::getColumn(int col) const
    {
        return Vector4(data[col], data[col + 4], data[col + 8], data[col + 12]);
    }

    Matrix3 Matrix4::getUpperLeft3x3() const
    {
        Matrix3 result;

        for (int row = 0; row < 3; ++row)
        {
            for (int col = 0; col < 3; ++col)
            {
                result.set(row, col, get(row, col));
            }
        }

        return result;
    }

    Vector3 Matrix4::getTranslation() const
    {
        return Vector3(get(0, 3), get(1, 3), get(2, 3));
    }

    void Matrix4::setTranslation(const Vector3 &translation)
    {
        set(0, 3, translation.x);
        set(1, 3, translation.y);
        set(2, 3, translation.z);
    }

    Matrix4 Matrix4::operator+(const Matrix4 &other) const
    {
        std::array<float, 16> result;
        for (int i = 0; i < 16; ++i)
        {
            result[i] = data[i] + other.data[i];
        }
        return Matrix4(result);
    }

    Matrix4 Matrix4::operator-(const Matrix4 &other) const
    {
        std::array<float, 16> result;
        for (int i = 0; i < 16; ++i)
        {
            result[i] = data[i] - other.data[i];
        }
        return Matrix4(result);
    }

    Matrix4 Matrix4::operator*(const Matrix4 &other) const
    {
        Matrix4 result;

        for (int row = 0; row < 4; ++row)
        {
            for (int col = 0; col < 4; ++col)
            {
                float sum = 0.0f;
                for (int i = 0; i < 4; ++i)
                {
                    sum += get(row, i) * other.get(i, col);
                }
                result.set(row, col, sum);
            }
        }

        return result;
    }

    Vector4 Matrix4::operator*(const Vector4 &vec) const
    {
        return Vector4(
            get(0, 0) * vec.x + get(0, 1) * vec.y + get(0, 2) * vec.z + get(0, 3) * vec.w,
            get(1, 0) * vec.x + get(1, 1) * vec.y + get(1, 2) * vec.z + get(1, 3) * vec.w,
            get(2, 0) * vec.x + get(2, 1) * vec.y + get(2, 2) * vec.z + get(2, 3) * vec.w,
            get(3, 0) * vec.x + get(3, 1) * vec.y + get(3, 2) * vec.z + get(3, 3) * vec.w);
    }

    Matrix4 Matrix4::operator*(float scalar) const
    {
        std::array<float, 16> result;
        for (int i = 0; i < 16; ++i)
        {
            result[i] = data[i] * scalar;
        }
        return Matrix4(result);
    }

    Matrix4 Matrix4::operator/(float scalar) const
    {
        float invScalar = 1.0f / scalar;
        return (*this) * invScalar;
    }

    Matrix4 &Matrix4::operator+=(const Matrix4 &other)
    {
        for (int i = 0; i < 16; ++i)
        {
            data[i] += other.data[i];
        }
        return *this;
    }

    Matrix4 &Matrix4::operator-=(const Matrix4 &other)
    {
        for (int i = 0; i < 16; ++i)
        {
            data[i] -= other.data[i];
        }
        return *this;
    }

    Matrix4 &Matrix4::operator*=(const Matrix4 &other)
    {
        *this = (*this) * other;
        return *this;
    }

    Matrix4 &Matrix4::operator*=(float scalar)
    {
        for (int i = 0; i < 16; ++i)
        {
            data[i] *= scalar;
        }
        return *this;
    }

    Matrix4 &Matrix4::operator/=(float scalar)
    {
        float invScalar = 1.0f / scalar;
        return (*this) *= invScalar;
    }

    bool Matrix4::operator==(const Matrix4 &other) const
    {
        for (int i = 0; i < 16; ++i)
        {
            if (std::abs(data[i] - other.data[i]) > EPSILON)
            {
                return false;
            }
        }
        return true;
    }

    bool Matrix4::operator!=(const Matrix4 &other) const
    {
        return !(*this == other);
    }

    float Matrix4::determinant() const
    {
        // Calculate determinant using cofactor expansion along the first row
        float cofactor00 = get(1, 1) * get(2, 2) * get(3, 3) +
                           get(1, 2) * get(2, 3) * get(3, 1) +
                           get(1, 3) * get(2, 1) * get(3, 2) -
                           get(1, 3) * get(2, 2) * get(3, 1) -
                           get(1, 2) * get(2, 1) * get(3, 3) -
                           get(1, 1) * get(2, 3) * get(3, 2);

        float cofactor01 = get(1, 0) * get(2, 2) * get(3, 3) +
                           get(1, 2) * get(2, 3) * get(3, 0) +
                           get(1, 3) * get(2, 0) * get(3, 2) -
                           get(1, 3) * get(2, 2) * get(3, 0) -
                           get(1, 2) * get(2, 0) * get(3, 3) -
                           get(1, 0) * get(2, 3) * get(3, 2);

        float cofactor02 = get(1, 0) * get(2, 1) * get(3, 3) +
                           get(1, 1) * get(2, 3) * get(3, 0) +
                           get(1, 3) * get(2, 0) * get(3, 1) -
                           get(1, 3) * get(2, 1) * get(3, 0) -
                           get(1, 1) * get(2, 0) * get(3, 3) -
                           get(1, 0) * get(2, 3) * get(3, 1);

        float cofactor03 = get(1, 0) * get(2, 1) * get(3, 2) +
                           get(1, 1) * get(2, 2) * get(3, 0) +
                           get(1, 2) * get(2, 0) * get(3, 1) -
                           get(1, 2) * get(2, 1) * get(3, 0) -
                           get(1, 1) * get(2, 0) * get(3, 2) -
                           get(1, 0) * get(2, 2) * get(3, 1);

        return get(0, 0) * cofactor00 - get(0, 1) * cofactor01 + get(0, 2) * cofactor02 - get(0, 3) * cofactor03;
    }

    Matrix4 Matrix4::inverse() const
    {
        // Calculate determinant
        float det = determinant();

        // Check if matrix is invertible
        if (std::abs(det) < EPSILON)
        {
            return Matrix4::identity(); // Return identity if not invertible
        }

        float invDet = 1.0f / det;

        Matrix4 result;

        // Calculate cofactors and adjugate
        for (int row = 0; row < 4; ++row)
        {
            for (int col = 0; col < 4; ++col)
            {
                // Calculate minor
                float minor = 0.0f;

                int r1 = (row + 1) % 4;
                int r2 = (row + 2) % 4;
                int r3 = (row + 3) % 4;

                int c1 = (col + 1) % 4;
                int c2 = (col + 2) % 4;
                int c3 = (col + 3) % 4;

                minor = get(r1, c1) * (get(r2, c2) * get(r3, c3) - get(r2, c3) * get(r3, c2)) -
                        get(r1, c2) * (get(r2, c1) * get(r3, c3) - get(r2, c3) * get(r3, c1)) +
                        get(r1, c3) * (get(r2, c1) * get(r3, c2) - get(r2, c2) * get(r3, c1));

                // Calculate cofactor (apply sign)
                float cofactor = ((row + col) % 2 == 0) ? minor : -minor;

                // Set value in adjugate (transposed cofactor matrix)
                result.set(col, row, cofactor * invDet);
            }
        }

        return result;
    }

    Matrix4 Matrix4::transpose() const
    {
        Matrix4 result;

        for (int row = 0; row < 4; ++row)
        {
            for (int col = 0; col < 4; ++col)
            {
                result.set(col, row, get(row, col));
            }
        }

        return result;
    }

    Matrix4 Matrix4::identity()
    {
        return Matrix4(
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f);
    }

    Matrix4 Matrix4::translation(const Vector3 &translation)
    {
        return translation(translation.x, translation.y, translation.z);
    }

    Matrix4 Matrix4::translation(float x, float y, float z)
    {
        return Matrix4(
            1.0f, 0.0f, 0.0f, x,
            0.0f, 1.0f, 0.0f, y,
            0.0f, 0.0f, 1.0f, z,
            0.0f, 0.0f, 0.0f, 1.0f);
    }

    Matrix4 Matrix4::scaling(const Vector3 &scale)
    {
        return scaling(scale.x, scale.y, scale.z);
    }

    Matrix4 Matrix4::scaling(float sx, float sy, float sz)
    {
        return Matrix4(
            sx, 0.0f, 0.0f, 0.0f,
            0.0f, sy, 0.0f, 0.0f,
            0.0f, 0.0f, sz, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f);
    }

    Matrix4 Matrix4::scaling(float scale)
    {
        return scaling(scale, scale, scale);
    }

    Matrix4 Matrix4::rotationX(float angle)
    {
        float c = std::cos(angle);
        float s = std::sin(angle);

        return Matrix4(
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, c, -s, 0.0f,
            0.0f, s, c, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f);
    }

    Matrix4 Matrix4::rotationY(float angle)
    {
        float c = std::cos(angle);
        float s = std::sin(angle);

        return Matrix4(
            c, 0.0f, s, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            -s, 0.0f, c, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f);
    }

    Matrix4 Matrix4::rotationZ(float angle)
    {
        float c = std::cos(angle);
        float s = std::sin(angle);

        return Matrix4(
            c, -s, 0.0f, 0.0f,
            s, c, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f);
    }

    Matrix4 Matrix4::rotation(float x, float y, float z)
    {
        // Convert Euler angles to rotation matrices
        Matrix4 rx = rotationX(x);
        Matrix4 ry = rotationY(y);
        Matrix4 rz = rotationZ(z);

        // Combine rotations: Z * Y * X
        return rz * ry * rx;
    }

    Matrix4 Matrix4::rotation(const Quaternion &q)
    {
        return Matrix4(q.toMatrix3());
    }

    Matrix4 Matrix4::perspective(float fov, float aspectRatio, float near, float far)
    {
        // Convert FOV from degrees to radians
        float fovRad = fov * DEG_TO_RAD;

        // Calculate height and width of the near plane
        float tanHalfFov = std::tan(fovRad * 0.5f);
        float height = 2.0f * near * tanHalfFov;
        float width = height * aspectRatio;

        // Calculate right, left, top, bottom of the near plane
        float right = width * 0.5f;
        float left = -right;
        float top = height * 0.5f;
        float bottom = -top;

        // Create perspective projection matrix
        float a = (right + left) / (right - left);
        float b = (top + bottom) / (top - bottom);
        float c = -(far + near) / (far - near);
        float d = -2.0f * far * near / (far - near);

        Matrix4 result;

        result.set(0, 0, 2.0f * near / (right - left));
        result.set(0, 2, a);

        result.set(1, 1, 2.0f * near / (top - bottom));
        result.set(1, 2, b);

        result.set(2, 2, c);
        result.set(2, 3, d);

        result.set(3, 2, -1.0f);
        result.set(3, 3, 0.0f);

        return result;
    }

    Matrix4 Matrix4::orthographic(float left, float right, float bottom, float top, float near, float far)
    {
        float width = right - left;
        float height = top - bottom;
        float depth = far - near;

        Matrix4 result;

        result.set(0, 0, 2.0f / width);
        result.set(0, 3, -(right + left) / width);

        result.set(1, 1, 2.0f / height);
        result.set(1, 3, -(top + bottom) / height);

        result.set(2, 2, -2.0f / depth);
        result.set(2, 3, -(far + near) / depth);

        return result;
    }

    Matrix4 Matrix4::lookAt(const Vector3 &eye, const Vector3 &target, const Vector3 &up)
    {
        // Calculate forward, right, and up vectors
        Vector3 forward = (target - eye).normalized();
        Vector3 right = up.cross(forward).normalized();
        Vector3 newUp = forward.cross(right);

        // Create view matrix
        Matrix4 result;

        result.set(0, 0, right.x);
        result.set(0, 1, right.y);
        result.set(0, 2, right.z);
        result.set(0, 3, -right.dot(eye));

        result.set(1, 0, newUp.x);
        result.set(1, 1, newUp.y);
        result.set(1, 2, newUp.z);
        result.set(1, 3, -newUp.dot(eye));

        result.set(2, 0, -forward.x);
        result.set(2, 1, -forward.y);
        result.set(2, 2, -forward.z);
        result.set(2, 3, forward.dot(eye));

        return result;
    }

} // namespace Engine