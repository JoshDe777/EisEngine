#pragma once

#include <string>
#include <assimp/quaternion.h>

namespace EisEngine {
    class Vector3;
    class Vector2;

    /// EisEngine's very own Quaternions!
    class Quaternion {
    public:
        /// Creates a new Quaternion
        /// @param x: The x value of this quaternion
        /// @param y: The y value of this quaternion
        /// @param z: The z value of this quaternion
        /// @param r: The r value of this quaternion
        explicit Quaternion(float x = 0, float y = 0, float z = 0, float r = 0): x(x), y(y), z(z), r(r) {}
        /// Creates a new Quaternion from an aiQuaternion object.
        /// @param q - aiQuaternion: the quaternion data used to create this quaternion.
        explicit Quaternion(const aiQuaternion& q) : x(q.x), y(q.y), z(q.z), r(q.w) {}
        /// Creates a new quaternion as an explicit vec3 + r composition.
        explicit Quaternion(const Vector3& axis, const float& r);

        /// Creates a new Quaternion from a 3D RotationVector.
        static Quaternion FromEulerXYZ(const Vector3& deg);
        /// Creates a new Quaternion as an angle on axis.
        static Quaternion FromAxisAngle(const Vector3& deg, const float& angle);

        /// The x value of this quaternion
        float x;
        /// The y value of this quaternion
        float y;
        /// The z value of this quaternion
        float z;
        /// The r value of this quaternion
        float r;

        # pragma region operators
        // interoperability with own vectors + strings
        operator Vector3() const;
        operator Vector2() const;
        operator std::string() const;

        // Quaternion math
        Quaternion operator+(Quaternion const &q) const;
        Quaternion operator-(Quaternion const &q) const;
        Quaternion operator*(Quaternion const &q) const;
        Quaternion operator*(float const& c) const;
        Quaternion operator*(int const& c) const;
        Vector3 operator*(const Vector3& v) const;

        Quaternion& operator+=(Quaternion const &q);
        Quaternion& operator-=(Quaternion const &q);
        Quaternion & operator*=(float const &c);
        Quaternion & operator*=(int const &c);
        #pragma endregion

        /// A quaternion providing no rotation\n -> (0, 0, 0, 1)
        static const Quaternion Identity;

        /// The absolute scale of the quaternion.
        /// @return a float representing the absolute value of the quaternion.
        float magnitude() const;

        /// The quaternion normalized to have the sum of its members squared be 1.
        /// If it is a 0 quaternion, it does not normalize.
        Quaternion normalized() const { 
            // issue warning if quaternion magnitude is 0
            if (this->magnitude() == 0) {
                DEBUG_WARN("Attempting to normalize an empty Quaternion!")
                return *this;
            }

            return (*this * (1/this->magnitude()));
        }

        /// The conjugated value of this quaternion.
        Quaternion conjugated() { return Quaternion(r, -x, -y, -z);}
    };
}
