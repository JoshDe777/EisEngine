namespace EisEngine {
    // extract euler rotation data from a matrix.
    inline Vector3 ConvertMatrixToEuler(const glm::mat4& mat) {
        glm::vec3 euler;
        glm::extractEulerAngleYXZ(mat, euler.y, euler.x, euler.z);

        return Vector3(
            Math::RadiansToDegrees(euler.x),
            Math::RadiansToDegrees(euler.y),
            Math::RadiansToDegrees(euler.z)
        );
    }
}
