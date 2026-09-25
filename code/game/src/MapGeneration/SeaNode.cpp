#include "SeaNode.h"

namespace Ocean_Outlaws::map {
    SeaNode::SeaNode(const int& id) : nodeID(id), data(), edges() { }

    float SeaNode::Heuristic(const SeaNode &goal) const {
        if(goal == *this)
            return 0;
        return Vector2::Distance(coordinates, goal.coordinates);
    }

    bool SeaNode::IsAdjacentTo(const Ocean_Outlaws::map::SeaNode &node) {
        for(auto* edge : edges)
            if(edge->Contains(node))
                return true;
        return false;
    }

    bool SeaNode::operator==(const Ocean_Outlaws::map::SeaNode &other) const { return nodeID == other.nodeID;}
}