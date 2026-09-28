namespace EisEngine {
    /// Provides a random float value x with min (incl.) lower equal to x lower than max (excl.)
    inline float RandomFloat(const float& min, const float& max) {
        std::random_device rnd;
        std::mt19937 gen(rnd());
        std::uniform_real_distribution<> dis(min, max);
        return static_cast<float>(dis(gen));
    }

    /// Provides a random integer value x within the range [min, max] (both inclusive).
    inline int RandomInt(const int& min, const int& max) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distr(min, max);
        return distr(gen);
    }
}
