#pragma once

#include <string>
#include <glm/glm.hpp>
#include <box2d/box2d.h>
#include <assimp/vector3.h>

namespace EisEngine {
    class Vector2;
    class Quaternion;

    /// The one and only 3-dimensional vector (x, y, z).
    class Vector3 {
    public:
        /// Creates a 3-dimensional vector.
        /// @param x - the x value of the vector. (right/left in world space)
        /// @param y - the y value of the vector. (up/down in world space)
        /// @param z - the z value of the vector. (forwards/backwards in world space)
        explicit Vector3(const float& x = 0, const float& y = 0, const float& z = 0) : x(x), y(y), z(z) {}
        /// Creates a 3-dimensional vector from an existing b2Vec2, setting z to 0.
        explicit Vector3(const b2Vec2 &v) : x(v.x), y(v.y), z(0) {}
        /// Creates a 3-dimensional vector from an existing glm vec3.
        explicit Vector3(const glm::vec3 &v):x(v.x), y(v.y), z(v.z) {}
        /// Creates a 3-dimensional vector from an existing assimp Vector3D.
        explicit Vector3(const aiVector3D& v) : x(v.x), y(v.y), z(v.z) { }
        /// Creates a 3-dimensional vector using a float distributed to all three dimensions.
        explicit Vector3(float& f) : x(f), y(f), z(f) {}

        /// The x-value of this vector (right/left in local space).
        float x;
        /// The y-value of this vector (up/down in local space).
        float y;
        /// The z-value of this vector (forward/backward in local space).
        float z;

        /// the size of the vector. returns the square root of all vector values squared.
        [[nodiscard]] float magnitude() const;
        /// Normalizes the vector to have the same direction but with magnitude 0.
        [[nodiscard]] Vector3 normalized() const;
        /// Clamps a Vector3's magnitude and returns a vector scaled to fit the given range.
        [[nodiscard]] Vector3 ClampMagnitude(const float& minMagnitude, const float& maxMagnitude);

        /// Calculates the distance between two Vector3's.
        [[nodiscard]] static float Distance(const Vector3& v1, const Vector3& v2);

        /// Calculates the dot product of two Vector3's.
        /// Represents the sum of the products of each vector axis.
        [[nodiscard]] static float DotProduct(const Vector3& v1, const Vector3& v2);

        /// Calculates the cross product of two Vector3's.
        [[nodiscard]] static Vector3 CrossProduct(const Vector3& v1, const Vector3& v2);

        /// Calculates the angle between both vectors in radians.
        [[nodiscard]] static float Angle(const Vector3& v1, const Vector3& v2);

        /// A Vector3 constant with values (0, 0, 0).
        static const Vector3 zero;
        /// A Vector3 constant with values (1, 1, 1).
        static const Vector3 one;
        /// A Vector3 constant with values (1, 0, 0), representing the positive x direction in world space.
        static const Vector3 right;
        /// A Vector3 constant with values (0, 1, 0), representing the positive y direction in world space.
        static const Vector3 up;
        /// A Vector3 constant with values (0, 0, -1), representing the positive z direction in world space.
        static const Vector3 forward;

        /// Rotates the vector by the given vector in degrees.
        [[nodiscard]] Vector3 Rotate(const Vector3& rotationAngles) const;

        #pragma region operators
        // interoperability operators
        operator Vector2() const;
        operator Quaternion() const;
        operator glm::vec3() const;
        operator glm::vec2() const;
        operator b2Vec2() const;
        operator std::string() const;

        // vector math
        Vector3 operator-() const;
        Vector3 operator+(Vector3 const &v)const;
        Vector3 operator-(Vector3 const &v)const;
        Vector3 operator*(float const &c) const;
        Vector3 operator*(int const &c)const;

        friend Vector3 operator*(const float& scalar, const Vector3& v);
        friend Vector3 operator*(const int& scalar, const Vector3& v);

        Vector3 operator*(const glm::mat4& m) const;

        Vector3 &operator+=(Vector3 const &v);
        Vector3 &operator-=(Vector3 const &v);
        Vector3 &operator*=(float const &c);
        Vector3 &operator*=(int const &c);

        // binary operations
        bool operator==(Vector3 const &v) const{ return x == v.x && y == v.y && z == v.z;}
        bool operator==(Vector2 const &v) const;
        bool operator!=(Vector3 const &v) const{ return !(*this == v);}
        bool operator!=(Vector2 const &v) const { return !(*this == v);}
        #pragma endregion
    };
}
