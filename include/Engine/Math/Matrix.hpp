#pragma once

#include <cmath>
#include <array>
#include "Engine/Math/Vector.hpp"

namespace Engine
{

    /**
     * @brief 3x3 matrix
     */
    class Matrix3
    {
    public:
        /**
         * @brief Default constructor (identity matrix)
         */
        Matrix3();

        /**
         * @brief Constructor from array
         * @param data 9 elements in row-major order
         */
        explicit Matrix3(const std::array<float, 9> &data);

        /**
         * @brief Constructor from individual elements (row-major order)
         */
        Matrix3(float m00, float m01, float m02,
                float m10, float m11, float m12,
                float m20, float m21, float m22);

        /**
         * @brief Gets an element by row and column
         * @param row Row index (0-2)
         * @param col Column index (0-2)
         * @return Element value
         */
        float get(int row, int col) const;

        /**
         * @brief Sets an element by row and column
         * @param row Row index (0-2)
         * @param col Column index (0-2)
         * @param value Element value
         */
        void set(int row, int col, float value);

        /**
         * @brief Gets the matrix data
         * @return Matrix data
         */
        const float *getData() const { return data.data(); }

        /**
         * @brief Gets a row of the matrix
         * @param row Row index (0-2)
         * @return Row vector
         */
        Vector3 getRow(int row) const;

        /**
         * @brief Gets a column of the matrix
         * @param col Column index (0-2)
         * @return Column vector
         */
        Vector3 getColumn(int col) const;

        /**
         * @brief Addition operator
         * @param other Matrix to add
         * @return Result of the addition
         */
        Matrix3 operator+(const Matrix3 &other) const;

        /**
         * @brief Subtraction operator
         * @param other Matrix to subtract
         * @return Result of the subtraction
         */
        Matrix3 operator-(const Matrix3 &other) const;

        /**
         * @brief Multiplication operator (matrix-matrix)
         * @param other Matrix to multiply by
         * @return Result of the multiplication
         */
        Matrix3 operator*(const Matrix3 &other) const;

        /**
         * @brief Multiplication operator (matrix-vector)
         * @param vec Vector to multiply by
         * @return Result of the multiplication
         */
        Vector3 operator*(const Vector3 &vec) const;

        /**
         * @brief Multiplication operator (scalar)
         * @param scalar Scalar to multiply by
         * @return Result of the multiplication
         */
        Matrix3 operator*(float scalar) const;

        /**
         * @brief Division operator (scalar)
         * @param scalar Scalar to divide by
         * @return Result of the division
         */
        Matrix3 operator/(float scalar) const;

        /**
         * @brief Addition assignment operator
         * @param other Matrix to add
         * @return Reference to this matrix
         */
        Matrix3 &operator+=(const Matrix3 &other);

        /**
         * @brief Subtraction assignment operator
         * @param other Matrix to subtract
         * @return Reference to this matrix
         */
        Matrix3 &operator-=(const Matrix3 &other);

        /**
         * @brief Multiplication assignment operator (matrix-matrix)
         * @param other Matrix to multiply by
         * @return Reference to this matrix
         */
        Matrix3 &operator*=(const Matrix3 &other);

        /**
         * @brief Multiplication assignment operator (scalar)
         * @param scalar Scalar to multiply by
         * @return Reference to this matrix
         */
        Matrix3 &operator*=(float scalar);

        /**
         * @brief Division assignment operator (scalar)
         * @param scalar Scalar to divide by
         * @return Reference to this matrix
         */
        Matrix3 &operator/=(float scalar);

        /**
         * @brief Equality operator
         * @param other Matrix to compare with
         * @return True if the matrices are equal, false otherwise
         */
        bool operator==(const Matrix3 &other) const;

        /**
         * @brief Inequality operator
         * @param other Matrix to compare with
         * @return True if the matrices are not equal, false otherwise
         */
        bool operator!=(const Matrix3 &other) const;

        /**
         * @brief Calculates the determinant of the matrix
         * @return Determinant
         */
        float determinant() const;

        /**
         * @brief Calculates the inverse of the matrix
         * @return Inverse matrix
         */
        Matrix3 inverse() const;

        /**
         * @brief Calculates the transpose of the matrix
         * @return Transposed matrix
         */
        Matrix3 transpose() const;

        /**
         * @brief Creates an identity matrix
         * @return Identity matrix
         */
        static Matrix3 identity();

        /**
         * @brief Creates a scaling matrix
         * @param scale Scale factors
         * @return Scaling matrix
         */
        static Matrix3 scaling(const Vector3 &scale);

        /**
         * @brief Creates a scaling matrix
         * @param sx X scale factor
         * @param sy Y scale factor
         * @param sz Z scale factor
         * @return Scaling matrix
         */
        static Matrix3 scaling(float sx, float sy, float sz);

        /**
         * @brief Creates a uniform scaling matrix
         * @param scale Scale factor
         * @return Scaling matrix
         */
        static Matrix3 scaling(float scale);

        /**
         * @brief Creates a rotation matrix around the X axis
         * @param angle Angle in radians
         * @return Rotation matrix
         */
        static Matrix3 rotationX(float angle);

        /**
         * @brief Creates a rotation matrix around the Y axis
         * @param angle Angle in radians
         * @return Rotation matrix
         */
        static Matrix3 rotationY(float angle);

        /**
         * @brief Creates a rotation matrix around the Z axis
         * @param angle Angle in radians
         * @return Rotation matrix
         */
        static Matrix3 rotationZ(float angle);

        /**
         * @brief Creates a rotation matrix from Euler angles
         * @param x Rotation around the X axis in radians
         * @param y Rotation around the Y axis in radians
         * @param z Rotation around the Z axis in radians
         * @return Rotation matrix
         */
        static Matrix3 rotation(float x, float y, float z);

        /**
         * @brief Creates a rotation matrix from a quaternion
         * @param q Quaternion
         * @return Rotation matrix
         */
        static Matrix3 rotation(const class Quaternion &q);

    private:
        /**
         * @brief Matrix data (row-major order)
         */
        std::array<float, 9> data;
    };

    /**
     * @brief 4x4 matrix
     */
    class Matrix4
    {
    public:
        /**
         * @brief Default constructor (identity matrix)
         */
        Matrix4();

        /**
         * @brief Constructor from array
         * @param data 16 elements in row-major order
         */
        explicit Matrix4(const std::array<float, 16> &data);

        /**
         * @brief Constructor from individual elements (row-major order)
         */
        Matrix4(float m00, float m01, float m02, float m03,
                float m10, float m11, float m12, float m13,
                float m20, float m21, float m22, float m23,
                float m30, float m31, float m32, float m33);

        /**
         * @brief Constructor from Matrix3 (extends to 4x4 with identity)
         * @param m 3x3 matrix
         */
        explicit Matrix4(const Matrix3 &m);

        /**
         * @brief Gets an element by row and column
         * @param row Row index (0-3)
         * @param col Column index (0-3)
         * @return Element value
         */
        float get(int row, int col) const;

        /**
         * @brief Sets an element by row and column
         * @param row Row index (0-3)
         * @param col Column index (0-3)
         * @param value Element value
         */
        void set(int row, int col, float value);

        /**
         * @brief Gets the matrix data
         * @return Matrix data
         */
        const float *getData() const { return data.data(); }

        /**
         * @brief Gets a row of the matrix
         * @param row Row index (0-3)
         * @return Row vector
         */
        Vector4 getRow(int row) const;

        /**
         * @brief Gets a column of the matrix
         * @param col Column index (0-3)
         * @return Column vector
         */
        Vector4 getColumn(int col) const;

        /**
         * @brief Gets the 3x3 upper-left submatrix
         * @return 3x3 submatrix
         */
        Matrix3 getUpperLeft3x3() const;

        /**
         * @brief Gets the translation component
         * @return Translation vector
         */
        Vector3 getTranslation() const;

        /**
         * @brief Sets the translation component
         * @param translation Translation vector
         */
        void setTranslation(const Vector3 &translation);

        /**
         * @brief Addition operator
         * @param other Matrix to add
         * @return Result of the addition
         */
        Matrix4 operator+(const Matrix4 &other) const;

        /**
         * @brief Subtraction operator
         * @param other Matrix to subtract
         * @return Result of the subtraction
         */
        Matrix4 operator-(const Matrix4 &other) const;

        /**
         * @brief Multiplication operator (matrix-matrix)
         * @param other Matrix to multiply by
         * @return Result of the multiplication
         */
        Matrix4 operator*(const Matrix4 &other) const;

        /**
         * @brief Multiplication operator (matrix-vector)
         * @param vec Vector to multiply by
         * @return Result of the multiplication
         */
        Vector4 operator*(const Vector4 &vec) const;

        /**
         * @brief Multiplication operator (scalar)
         * @param scalar Scalar to multiply by
         * @return Result of the multiplication
         */
        Matrix4 operator*(float scalar) const;

        /**
         * @brief Division operator (scalar)
         * @param scalar Scalar to divide by
         * @return Result of the division
         */
        Matrix4 operator/(float scalar) const;

        /**
         * @brief Addition assignment operator
         * @param other Matrix to add
         * @return Reference to this matrix
         */
        Matrix4 &operator+=(const Matrix4 &other);

        /**
         * @brief Subtraction assignment operator
         * @param other Matrix to subtract
         * @return Reference to this matrix
         */
        Matrix4 &operator-=(const Matrix4 &other);

        /**
         * @brief Multiplication assignment operator (matrix-matrix)
         * @param other Matrix to multiply by
         * @return Reference to this matrix
         */
        Matrix4 &operator*=(const Matrix4 &other);

        /**
         * @brief Multiplication assignment operator (scalar)
         * @param scalar Scalar to multiply by
         * @return Reference to this matrix
         */
        Matrix4 &operator*=(float scalar);

        /**
         * @brief Division assignment operator (scalar)
         * @param scalar Scalar to divide by
         * @return Reference to this matrix
         */
        Matrix4 &operator/=(float scalar);

        /**
         * @brief Equality operator
         * @param other Matrix to compare with
         * @return True if the matrices are equal, false otherwise
         */
        bool operator==(const Matrix4 &other) const;

        /**
         * @brief Inequality operator
         * @param other Matrix to compare with
         * @return True if the matrices are not equal, false otherwise
         */
        bool operator!=(const Matrix4 &other) const;

        /**
         * @brief Calculates the determinant of the matrix
         * @return Determinant
         */
        float determinant() const;

        /**
         * @brief Calculates the inverse of the matrix
         * @return Inverse matrix
         */
        Matrix4 inverse() const;

        /**
         * @brief Calculates the transpose of the matrix
         * @return Transposed matrix
         */
        Matrix4 transpose() const;

        /**
         * @brief Creates an identity matrix
         * @return Identity matrix
         */
        static Matrix4 identity();

        /**
         * @brief Creates a translation matrix
         * @param translation Translation vector
         * @return Translation matrix
         */
        static Matrix4 translation(const Vector3 &translation);

        /**
         * @brief Creates a translation matrix
         * @param x X translation
         * @param y Y translation
         * @param z Z translation
         * @return Translation matrix
         */
        static Matrix4 translation(float x, float y, float z);

        /**
         * @brief Creates a scaling matrix
         * @param scale Scale factors
         * @return Scaling matrix
         */
        static Matrix4 scaling(const Vector3 &scale);

        /**
         * @brief Creates a scaling matrix
         * @param sx X scale factor
         * @param sy Y scale factor
         * @param sz Z scale factor
         * @return Scaling matrix
         */
        static Matrix4 scaling(float sx, float sy, float sz);

        /**
         * @brief Creates a uniform scaling matrix
         * @param scale Scale factor
         * @return Scaling matrix
         */
        static Matrix4 scaling(float scale);

        /**
         * @brief Creates a rotation matrix around the X axis
         * @param angle Angle in radians
         * @return Rotation matrix
         */
        static Matrix4 rotationX(float angle);

        /**
         * @brief Creates a rotation matrix around the Y axis
         * @param angle Angle in radians
         * @return Rotation matrix
         */
        static Matrix4 rotationY(float angle);

        /**
         * @brief Creates a rotation matrix around the Z axis
         * @param angle Angle in radians
         * @return Rotation matrix
         */
        static Matrix4 rotationZ(float angle);

        /**
         * @brief Creates a rotation matrix from Euler angles
         * @param x Rotation around the X axis in radians
         * @param y Rotation around the Y axis in radians
         * @param z Rotation around the Z axis in radians
         * @return Rotation matrix
         */
        static Matrix4 rotation(float x, float y, float z);

        /**
         * @brief Creates a rotation matrix from a quaternion
         * @param q Quaternion
         * @return Rotation matrix
         */
        static Matrix4 rotation(const class Quaternion &q);

        /**
         * @brief Creates a perspective projection matrix
         * @param fov Field of view (in radians)
         * @param aspectRatio Aspect ratio (width / height)
         * @param near Near clipping plane
         * @param far Far clipping plane
         * @return Perspective projection matrix
         */
        static Matrix4 perspective(float fov, float aspectRatio, float near, float far);

        /**
         * @brief Creates an orthographic projection matrix
         * @param left Left clipping plane
         * @param right Right clipping plane
         * @param bottom Bottom clipping plane
         * @param top Top clipping plane
         * @param near Near clipping plane
         * @param far Far clipping plane
         * @return Orthographic projection matrix
         */
        static Matrix4 orthographic(float left, float right, float bottom, float top, float near, float far);

        /**
         * @brief Creates a look-at view matrix
         * @param eye Camera position
         * @param target Target position
         * @param up Up vector
         * @return Look-at view matrix
         */
        static Matrix4 lookAt(const Vector3 &eye, const Vector3 &target, const Vector3 &up);

    private:
        /**
         * @brief Matrix data (row-major order)
         */
        std::array<float, 16> data;
    };

} // namespace Engine