#include "SeaGraphEdge.h"

#include "SeaNode.h"

namespace Ocean_Outlaws::map {
    SeaGraphEdge::SeaGraphEdge(SeaNode *nodeA, SeaNode *nodeB) :
    A(nodeA), B(nodeB) { }

    bool SeaGraphEdge::Contains(const SeaNode &node) const {
        // if A is nullptr, return false if edge is completely undefined, otherwise compare with B
        if(!A){
            if(!B)
                return false;
            else
                return node == *B;
        }
        // if B is nullptr (A isn't thanks to else case), compare with A
        else if (!B)
            return node == *A;
        // if neither is nullptr compare with both.
        return (node == *A || node == *B);
    }

    float SeaGraphEdge::Cost() const { return Vector2::Distance(A->coordinates, B->coordinates);}
}
