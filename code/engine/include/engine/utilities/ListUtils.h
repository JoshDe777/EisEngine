namespace EisEngine {
    /// Determines whether a vector contains a specific item.
    template<typename T>
    bool ListContains(const std::vector<T>& list, const T& item) {
        return std::find(list.begin(), list.end(), item) != list.end();
    }
}
