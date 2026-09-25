#pragma once

#include "../MapGeneration/SeaNode.h"
#include "../Enums.h"

namespace Ocean_Outlaws::AI {
    using SeaNode = Ocean_Outlaws::map::SeaNode;
    /// \n A static class providing several path finding options for the enemy AI.
    class Pathfinding {
    public:
        /// \n An A-Star algorithm determining a key node path from the start to target nodes.
        /// \n A key node path filters in only every xth node in the path, allowing for smoother movement.
        static std::vector<SeaNode*> AStarToNode(SeaNode* start, SeaNode* target, const int& pathStep);

        /// \n Used for patrol route establishment. Use only diagonal directions (NW, NE, SE, SW).
        static SeaNode* FindFurthestSeaNode(SeaNode* currentNode, const CardinalDirection& direction,
                                            const int& iterations);

        /// \n Retrieves the sea node closest to the given set of coordinates.
        static SeaNode* FindClosestSeaNode(const Vector3& coordinates);
    private:
        /// \n Calculates the A star score using the provided data.
        static float Score(const float& costToNode, const float& heuristic);
        /// \n Selects the best next node in the boundary nodes vector.
        static SeaNode& SelectNext(std::vector<SeaNode*>& nodes);
        /// \n Gets the rough direction in which a node is placed compared to the end node.
        static CardinalDirection GetDirection(const SeaNode* start, const SeaNode* end);
        /// \n Sets the edge weight for the FindFurthestSeaNode algorithm,
        /// incentivizing movement into the desired direction.
        static float SetEdgeWeight(const CardinalDirection& desiredDirection, const CardinalDirection& direction);
    };
}
