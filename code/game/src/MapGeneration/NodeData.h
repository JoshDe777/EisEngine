#pragma once

#include <limits>

namespace Ocean_Outlaws::map {
    class SeaNode;

    struct NodeData {
    public:
        float costToNode = std::numeric_limits<float>::max();
        float score = std::numeric_limits<float>::max();
        SeaNode* predecessor = nullptr;
        bool visited = false;

        void Reset(){
            costToNode = std::numeric_limits<float>::max();
            score = std::numeric_limits<float>::max();
            predecessor = nullptr;
            visited = false;
        }
    };
}
