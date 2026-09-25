#pragma once

#include <map>
#include <string>
#include <any>
#include <EisEngine.h>
#include "BTNode.h"

namespace Ocean_Outlaws::AI {
    /// \n A base class for AI behaviour trees.
    /// Iterates through behaviour 'nodes' like a tree structure based on the feedback provided after node execution.
    class BehaviourTree : public Script {
    public:
        /// \n Creates a new behaviour tree.
        BehaviourTree(Game& engine, guid_t owner);

        /// \n Access the data dictionary with the key and a reference to the object the result will be assigned to.
        /// @return a bool determining whether the data fetching was successful.
        template<typename T>
        bool TryGetData(const std::string& key, T& val){
            auto it = dataContext.find(key);
            auto success = it != dataContext.end();
            if(success){
                try {
                    val = std::any_cast<T>(it->second);
                    return true;
                } catch (const std::bad_any_cast&) {
                    return false;
                }
            }
            return success;
        }

        /// \n Set the data allocated to the key value.
        /// Assigning data to a previously assigned key WILL overwrite previously defined data.
        template<typename T>
        void SetData(const std::string& key, T val)
        { dataContext[key] = val;}

        /// \n Removes data allocated to the provided key.
        void RemoveData(const std::string& key);

        /// \n returns the root node of the behaviour tree.
        BTNode* GetRoot() const { return root;}
    protected:
        /// \n An abstract method called to construct a behaviour tree.
        virtual BTNode* BuildTree() = 0;
        /// \n A function called when a component is intentionally deleted.
        void Invalidate() override;
        /// \n A function called once at the beginning of a script's lifetime.
        void Start() override;
    private:
        /// \n The root node of the behaviour tree.
        BTNode* root = nullptr;
        /// \n A dictionary of data to be passed across behaviour nodes.
        std::map<std::string, std::any> dataContext;
    };
}
