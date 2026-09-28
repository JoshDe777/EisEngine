namespace EisEngine {
    /// Produces a string of the given vector of floats in the format [a, b, ..., n].
    inline std::string to_string(const std::vector<float>& v) {
        std::ostringstream oss;
        oss.precision(6);

        oss << "[";

        for (size_t i = 0; i < v.size(); ++i) {
            oss << v[i];
            if (i != v.size() - 1)
                oss << ", ";
        }

        oss << "]";

        return (std::string)oss.str();
    }

    /// Produces a string of the given vector of unsigned ints in the format [a, b, ..., n].
    inline std::string to_string(const std::vector<unsigned int>& v) {
        std::ostringstream oss;
        oss.precision(6);

        oss << "[";

        for (size_t i = 0; i < v.size(); ++i) {
            oss << v[i];
            if (i != v.size() - 1)
                oss << ", ";
        }

        oss << "]";

        return (std::string)oss.str();
    }

    /// Produces a string of the given b2Vec2, in the format (x, y)
    inline std::string to_string(const b2Vec2& v) {
        std::ostringstream oss;
        oss.precision(6);
        oss << "(" << v.x << ", " << v.y << ")";
        return (std::string)oss.str();
    }

    /// Produces a string of the provided vector of b2Vec2's, in the format [(x1, y1), (x2, y2), ..., (xn, yn)].
    inline std::string to_string(const std::vector<b2Vec2>& v) {
        std::ostringstream oss;
        oss.precision(6);

        oss << "[";

        for (size_t i = 0; i < v.size(); ++i) {
            oss << EisEngine::to_string(v[i]);
            if (i != v.size() - 1)
                oss << ", ";
        }

        oss << "]";

        return (std::string)oss.str();
    }
}