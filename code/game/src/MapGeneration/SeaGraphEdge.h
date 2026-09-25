#pragma once

namespace Ocean_Outlaws::map {
    class SeaNode;

    /// \n An undirected edge in the sea tile map graph.
    struct SeaGraphEdge {
    public:
        /// \n An undirected edge in the sea tile map graph.
        explicit SeaGraphEdge(SeaNode* nodeA, SeaNode* nodeB);

        /// \n Node A connected by this edge.
        SeaNode* A;
        /// \n Node B connected by this edge.
        SeaNode* B;

        /// \n Returns whether this edge connects to the provided node.
        [[nodiscard]] bool Contains(const SeaNode& node) const;
        /// \n Returns the cost of travelling along this edge.
        [[nodiscard]] float Cost() const;
    };
}
