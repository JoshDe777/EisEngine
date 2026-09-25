#pragma once

#include <EisEngine.h>

#include "SeaGraphEdge.h"
#include "NodeData.h"

namespace Ocean_Outlaws::map {
    /// \n A node in the ocean tile map.
    class SeaNode {
    public:
        /// \n The list of undirected edges connecting to this node.
        std::vector<SeaGraphEdge *> edges;
        /// \n The 2D coordinates of this tile.
        Vector2 coordinates = Vector2::zero;
        /// \n The node's context.
        NodeData data;

        /// \n A node in the ocean tile map.
        explicit SeaNode(const int& id);
        /// \n The heuristic value attached to this node.
        [[nodiscard]] float Heuristic(const SeaNode& goal) const;
        /// \n Returns whether a node is adjacent to a given node.
        bool IsAdjacentTo(const SeaNode& node);

        bool operator==(const SeaNode& other) const;
    private:
        /// \n The unique ID of the node.
        const int nodeID;
    };
}
