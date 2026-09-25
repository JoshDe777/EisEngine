#include "Pathfinding.h"

#include "../GameObject.h"
#include "../Scripts/MapGenerator.h"

namespace Ocean_Outlaws::AI {
    std::vector<SeaNode*> Pathfinding::AStarToNode(SeaNode* start, SeaNode* target, const int& pathStep) {
        GameObject::FindObjectOfType<MapGenerator>()->seaNodes.ResetAllData();
        start->data.costToNode = 0;
        start->data.score = Score(start->data.costToNode, start->Heuristic(*target));

        std::vector<SeaNode*> boundaryNodes = {start};
        bool reachedTarget = false;

        // as long as there is a path available
        while (!boundaryNodes.empty()){
            auto& current = SelectNext(boundaryNodes);          // select new frontier node

            if(current == *target){
                reachedTarget = true;
                break;
            }

            current.data.visited = true;
            boundaryNodes.erase(std::remove(boundaryNodes.begin(), boundaryNodes.end(), &current),
                                boundaryNodes.end());

            // go through each edge
            for(auto e : current.edges){
                auto neighbor = e->A == &current ? e->B : e->A;

                if(neighbor->data.visited)
                    continue;

                // calculate A* score
                auto costToNext = current.data.costToNode + e->Cost();
                auto newScore = Score(costToNext, neighbor->Heuristic(*target));

                if(newScore < neighbor->data.score){
                    neighbor->data.predecessor = &current;
                    neighbor->data.score = newScore;
                    neighbor->data.costToNode = costToNext;

                    if(!ListContains(boundaryNodes, neighbor))
                        boundaryNodes.push_back(neighbor);
                }
            }
        }

        if(!reachedTarget){
            DEBUG_WARN("Could not find valid path.")
            return { };
        }

        std::vector<SeaNode*> rawPath = {};

        auto current = target;
        while (current != nullptr){
            rawPath.push_back(current);
            current = current->data.predecessor;
        }

        // curate through the raw path and only include every xth node.
        if(rawPath.empty())
            return rawPath;

        std::vector<SeaNode*> keyNodePath = { };

        // include every xth node. With the modulo work, it 100% includes the target node, which should be rawPath[0].
        for(int i = (int) rawPath.size() - 1; i >= 0 ; i--){
            // unless it's the nth node, and excluding the start node in case it adds up, skip.
            if(i % pathStep != 0 || rawPath[i] == start)
                continue;
            keyNodePath.emplace_back(rawPath[i]);
        }

        return keyNodePath;
    }

    float Pathfinding::Score(const float &costToNode, const float &heuristic) { return costToNode + heuristic;}

    SeaNode& Pathfinding::SelectNext(std::vector<SeaNode *> &nodes) {
        assert(!nodes.empty());

        SeaNode* next = nullptr;
        auto lowestScore = std::numeric_limits<float>::max();

        for(auto node : nodes){
            if(node->data.score < lowestScore){
                lowestScore = node->data.score;
                next = node;
            }
        }
        if(!next)
            DEBUG_RUNTIME_ERROR("No options given!")

        return *next;
    }

    SeaNode* Pathfinding::FindFurthestSeaNode(SeaNode *currentNode, const CardinalDirection &direction,
                                              const int &iterations) {
        std::vector<CardinalDirection> acceptableDirections = { NORTH_WEST, NORTH_EAST, SOUTH_WEST, SOUTH_EAST };

        if (!ListContains(acceptableDirections, direction))
            return nullptr;

        if (iterations <= 0)
            return currentNode;

        // evaluate weight to neighbors, saving lightest edge
        SeaNode* closestNeighbor = nullptr;
        float lowestRecordedWeight = std::numeric_limits<float>::max();
        for(auto edge : currentNode->edges)
        {
            auto neighbor = edge->A == currentNode ? edge->A : edge->B;

            // if the weight of the edge between this node and the current neighbor is the lowest, set this neighbor as the preferred target node
            if (SetEdgeWeight(direction, GetDirection(currentNode, neighbor)) < lowestRecordedWeight)
                closestNeighbor = neighbor;
        }

        // call function from the lightest node recursively with iterations - 1
        return FindFurthestSeaNode(closestNeighbor, direction, iterations - 1);
    }

    SeaNode *Pathfinding::FindClosestSeaNode(const Vector3 &coordinates) {
        auto map = GameObject::FindObjectOfType<MapGenerator>();
        return map->FindClosestNode((Vector2) coordinates);
    }

    CardinalDirection
    Pathfinding::GetDirection(const SeaNode *start, const SeaNode *end) {
        bool north = end->coordinates.y > start->coordinates.y;
        bool east = end->coordinates.x > start->coordinates.x;
        bool south = end->coordinates.y < start->coordinates.y;
        bool west = end->coordinates.x < start->coordinates.x;

        // disclaimer: this system only works because coordinates are INTEGERS! if they weren't, it would be heavily unreliable! (since even .1 offset would negate any direct course
        // -> e.g. xi = 1, xj = 1.001 would not be considered going straight north or south, but NE or SE

        if (north)
        {
            if (east)
                return NORTH_EAST;
            else if (west)
                return NORTH_WEST;
            else
                return NORTH;
        }
        else if (south)
        {
            if (east)
                return SOUTH_EAST;
            else if (west)
                return SOUTH_WEST;
            else
                return SOUTH;
        }
        else
        {
            if (east)
                return EAST;
            else
                return WEST;
        }
    }

    float Pathfinding::SetEdgeWeight(const CardinalDirection &desiredDirection,
                                     const CardinalDirection &direction) {
        int xw = (int)((float)desiredDirection / 2);    // -> clamp all values between 0 & 7, 0 being East and 7 South-East (to disregard in-between directions e.g. NNW)
        int xMax = 7;   // -> max value of the enum, which is -1 % 8 = 7
        int x = (int)((float)direction / 2);    // -> new value given to current cardinal direction, also clamped between 0 & 7.

        int y = (x + (xMax - xw)) % 8;  // -> readjust enum values to make the desired direction worth 0; here y is basically the enum value of direction compared to the desired dir
        if (y > 4)  // since it's made to be at a distance from the desired direction, values need to be clamped between 0 & 4, 4 being the polar opposite.
        {
            int dist = 4 - y;   // solved by calculating the distance from 4 and setting y to 4 - dist (the further from 4, the closer to 0)
            y = 4 + dist;
        }
// 1 being the default value, adds a modifier in increments of 1/3 between [-2/3; 2/3] to the weight, incentivizing going in the right dir and penalizing going in the opposite direction.
        return 1 - (y - 2) / 3;
    }
}
