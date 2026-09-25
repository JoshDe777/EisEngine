#pragma once

#include "SeaNode.h"
#include "SeaGraphEdge.h"

namespace Ocean_Outlaws{
    namespace scripts { class MapGenerator;}

    namespace map {
        /// \n The graph of navigable water tiles.
        class SeaNodeGraph {
            using MapGenerator = Ocean_Outlaws::scripts::MapGenerator;
        public:
            /// \n The graph of navigable water tiles.
            explicit SeaNodeGraph(MapGenerator& map) : mapGenerator(map) { }

            /// \n The list of nodes composing the graph.
            std::vector<SeaNode> nodes = {};
            /// \n The list of edges connecting the various nodes.
            std::list<SeaGraphEdge> edges = {};

            MapGenerator& mapGenerator;

            void Reset();

            void AssignNeighbors();
            void AddNeighborIfExistsIn(SeaNode* node, const int& x, const int& y);
            static void PairNodes(SeaGraphEdge* edge);

            void ResetAllData();
            static Vector3 SeaNodeToWorldPos(SeaNode* node);
            SeaNode* FindClosestNode(const Vector2& coords);
            SeaNode* FindAt(const int& x, const int& y);
        };
    }
}
