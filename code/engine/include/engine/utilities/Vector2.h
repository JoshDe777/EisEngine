#pragma once

#include <string>
#include <glm/glm.hpp>
#include <box2d/box2d.h>
#include <assimp/vector3.h>

namespace EisEngine {
    class Vector3;
    class Quaternion;

    /// The timeless two-dimensional vector (x, y).
    class Vector2 {
    public:
        /// Creates a 2-dimensional vector.
        /// @param x - float: the x value of the vector (right/left in world space).
        /// @param y - float: the y value of the vector (up/down in world space).
        explicit Vector2(const float& x = 0, const float& y = 0) : x(x), y(y) {}
        /// Creates a 2-dimensional vector from an existing b2Vec2.
        explicit Vector2(b2Vec2 const &v): x(v.x), y(v.y){}
        /// Creates a 2-dimensional vector from an existing glm::vec2
        explicit Vector2(glm::vec2 const &v) : x(v.x), y(v.y) {}
        /// Creates a 2-dimensional vector from an existing aiVector3D.
        explicit Vector2(const aiVector3D& v) : x(v.x), y(v.y) {}
        /// Creates a 2-dimensional vector from an existing Vector2.
        Vector2(Vector2 const &v) = default;

        /// The x value of the vector (right/left in world space).
        float x;
        /// The y value of the vector (up/down in world space).
        float y;

        /// the size of the vector. returns the square root of all vector values squared.
        [[nodiscard]] float magnitude() const;

        /// Normalizes the vector to have the same direction but with magnitude 0.
        [[nodiscard]] Vector2 normalized() const;

        /// Clamps a Vector2's magnitude and returns a vector scaled to fit the given range.
        [[nodiscard]] Vector2 ClampMagnitude(const float& minMagnitude, const float& maxMagnitude);

        /// Calculates the distance between two Vector2's.
        [[nodiscard]] static float Distance(const Vector2& v1, const Vector2& v2);

        /// Calculates the dot product of two Vector2's.
        /// Represents the sum of the products of each vector axis.
        [[nodiscard]] static float DotProduct(const Vector2& v1, const Vector2& v2);

        /// Calculates the cross product of two Vector2's.
        [[nodiscard]] static float CrossProduct(const Vector2& v1, const Vector2& v2);

        /// Calculates the angle between the provided Vector2's in radians.
        [[nodiscard]] static float Angle(const Vector2& v1, const Vector2& v2);

        /// A Vector2 constant equal to (0, 0).
        static const Vector2 zero;
        /// A Vector2 constant equal to (1, 1).
        static const Vector2 one;
        /// A Vector2 constant equal to (1, 0).
        static const Vector2 right;
        /// A Vector2 constant equal to (0, 1).
        static const Vector2 up;

        /// Rotates the vector by the given angle in degrees.
        [[nodiscard]] Vector2 Rotate(const float& angle) const;
    
        #pragma region operators
        // interoperability with own, OpenGL and box2D vectors
        operator Vector3() const;
        operator Quaternion() const;
        operator glm::vec3() const;
        operator glm::vec2() const;
        operator b2Vec2() const;

        operator std::string() const;
        
        // vector math
        Vector2 operator-() const;
        Vector2 operator+(Vector2 const &v) const;
        Vector2 operator-(Vector2 const &v) const;
        Vector2 operator*(float const &c) const;
        Vector2 operator*(int const &c) const;
        Vector2 operator/(float const &c) const;
        Vector2 operator/(int const &c) const;

        friend Vector2 operator*(float scalar, const Vector2& v);
        friend Vector2 operator*(int scalar, const Vector2& v);
        friend Vector2 operator/(float scalar, const Vector2& v);
        friend Vector2 operator/(int scalar, const Vector2& v);

        Vector2 &operator+=(Vector2 const &v);
        Vector2 &operator-=(Vector2 const &v);
        Vector2 &operator*=(float const &c);
        Vector2 &operator*=(int const &c);

        bool operator==(Vector2 const &v) const;
        bool operator!=(Vector2 const &v) const;
        #pragma endregion
    };
}
