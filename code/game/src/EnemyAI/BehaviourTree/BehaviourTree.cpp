#include "BehaviourTree.h"

namespace Ocean_Outlaws::AI {
    BehaviourTree::BehaviourTree(EisEngine::Game &engine, EisEngine::ecs::guid_t owner) : Script(engine, owner) { }

    void BehaviourTree::RemoveData(const std::string &key) {
        auto it = dataContext.find(key);
        if(it == dataContext.end())
            return;
        dataContext.erase(it);
    }

    void BehaviourTree::Start() { root = BuildTree();}

    void BehaviourTree::Invalidate() {
        root->Deconstruct();
        Script::Invalidate();
    }
}