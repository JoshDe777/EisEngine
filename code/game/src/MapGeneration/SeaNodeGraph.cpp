#include "SeaNodeGraph.h"

#include "../Scripts/MapGenerator.h"

namespace Ocean_Outlaws::map{
    void SeaNodeGraph::Reset() {
        edges.clear();
        nodes.clear();
    }

    void SeaNodeGraph::AssignNeighbors() {
        for(auto& seaNode : nodes){
            int x = (int) seaNode.coordinates.x;
            int y = (int) seaNode.coordinates.y;

            auto seaNodePointer = &seaNode;

            // north
            AddNeighborIfExistsIn(seaNodePointer, x, y+1);
            // north-west
            AddNeighborIfExistsIn(seaNodePointer, x-1, y+1);
            // west
            AddNeighborIfExistsIn(seaNodePointer, x-1, y);
            // south-west
            AddNeighborIfExistsIn(seaNodePointer, x-1, y-1);
            // south
            AddNeighborIfExistsIn(seaNodePointer, x, y-1);
            // south-east
            AddNeighborIfExistsIn(seaNodePointer, x+1, y-1);
            // east
            AddNeighborIfExistsIn(seaNodePointer, x+1, y);
            // north-east
            AddNeighborIfExistsIn(seaNodePointer, x+1, y+1);
        }
    }

    void SeaNodeGraph::AddNeighborIfExistsIn(SeaNode *node, const int &x, const int &y) {
        // discards out of bounds coordinates immediately
        if (x < 0 || y < 0 || x >= mapGenerator.mapSize || y >= mapGenerator.mapSize)
            return;

        auto neighborNode = FindAt(x, y);
        if(neighborNode && !neighborNode->IsAdjacentTo(*node)){
            auto* edge = &edges.emplace_back(node, neighborNode);
            PairNodes(edge);
        }
    }

    void SeaNodeGraph::PairNodes(SeaGraphEdge *edge) {
        edge->A->edges.emplace_back(edge);
        edge->B->edges.emplace_back(edge);
    }

    void SeaNodeGraph::ResetAllData() {
        for (auto &node: nodes)
            node.data.Reset();
    }

    Vector3 SeaNodeGraph::SeaNodeToWorldPos(Ocean_Outlaws::map::SeaNode *node) {
        return Vector3(
                2 * node->coordinates.x,
                2 * node->coordinates.y,
                0
                );
    }

    SeaNode *SeaNodeGraph::FindAt(const int &x, const int &y) {
        auto coords = Vector2((float) x, (float) y);
        for(auto& node : nodes){
            if(node.coordinates == coords)
                return &node;
        }
        return nullptr;
    }

    SeaNode *SeaNodeGraph::FindClosestNode(const Vector2& coords) {
        auto adjustedCoords = coords / 2;

        SeaNode* closestNode = nullptr;
        auto closestNodeDist = std::numeric_limits<float>::max();

        for(auto& node : nodes){
            auto dist = Vector2::Distance(adjustedCoords, node.coordinates);
            if(dist < closestNodeDist){
                closestNodeDist = dist;
                closestNode = &node;
            }
        }
        return closestNode;
    }
}
