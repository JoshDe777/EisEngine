#pragma once

#include <map>
#include <vector>
#include <memory>
#include <typeinfo>
#include <utility>
#include <functional>
#include <string>
#include "engine/ecs/ecs.h"
#include "engine/ecs/Component.h"

namespace EisEngine {
    class Game;

    namespace ecs {
        /// Represents the Component management system.
        /// Handles allocating and deallocating components to entities as well as the deletion process for components of all kind.
        class ComponentManager {
        public:
            /// Creates a @a ComponentManager instance
            /// @param engine a reference to the game's instance.
            explicit ComponentManager(Game &engine);

            /// Creates a component of the given type and assigns it to an entity.
            /// @return Component* - a pointer to the newly created Component.
            template<typename C, typename ...Args>
            [[nodiscard]] C *addComponent(guid_t owner, Args ...args){
                auto& container = containers[typeid(C).hash_code()];
                auto component = std::make_unique<C>(engine, owner, args...);
                container[owner] = std::move(component);

                return getComponent<C>(owner);
            }

            /// Gets a Component of the given type from the specified entity.
            /// @return Component* - a pointer to the component if found on the owner entity.
            /// \n Returns a nullptr if no component was found.
            template<typename C>
            C *getComponent(guid_t owner){
                // return nullptr if no component of given type or if none assigned to the owner id
                if (!hasComponentOfType<C>() ||
                        containers.at(typeid(C).hash_code()).find(owner) ==
                        containers.at(typeid(C).hash_code()).end()
                        )
                    return nullptr;
                try {
                    return reinterpret_cast<C*>(containers.at(typeid(C).hash_code()).at(owner).get());
                }
                catch (std::exception &e){
                    // debug message that states exactly which component type broke the cast.
                    DEBUG_ERROR("ComponentManager::getComponent<" + (std::string) typeid(C).name() + ">" + e.what())
                    return nullptr;
                }
            }

            /// Iterates through all components of a given type and executes a function on all of them.
            /// @param C - the type of Component to be iterated through.
            /// @param f - a function with return type @a null, and parameter type C& (reference of the given component type) that will be executed on all components.
            template<typename C>
            void forEachComponent(std::function<void(C&)> f){
                // return early if there are no components of the provided type.
                if (!hasComponentOfType<C>())
                    return;

                // iterates through all components of type C and feeds a reference to f.
                for(const auto &[_, component]: containers.at(typeid(C).hash_code())) {
                    // I forgot why the reinterpret cast is necessary. I'm assuming because it doesn't do the conversion automatically? 
                    // This code was written over 6 months ago when I wrote this comment...
                    f(*reinterpret_cast<C*>(component.get()));
                }
            }


            /// Unbinds a component from its entity, and deletes the component object and its associated data.
            /// @param entityID - the unique ID of the entity whose component is to be removed.
            template<typename C>
            void removeComponent(guid_t entityID){
                auto& componentContainer = containers[typeid(C).hash_code()];
                const auto &component = componentContainer.find(entityID);
                const auto componentExistsInContainer = component != componentContainer.end();

                if(componentExistsInContainer) {
                    component->second->deleted = true;
                    component->second->Invalidate();
                    componentContainer.erase(entityID);
                }
            }

            /// Returns each component assigned to the given entity.
            /// ASSUMPTION ONLY ONE COMPONENT OF ANY TYPE PER ENTITY!
            std::vector<Component*> getEachComponentOfEntity(guid_t entityID){
                std::vector<Component*> components = {};
                for(auto &[componentTypeID, componentContainer] : containers){
                    const auto& component = componentContainer.find(entityID);
                    if(component != componentContainer.end())
                        components.emplace_back(&*component->second);
                }
                return components;
            }

            /// Removes all components from the given entity.
            /// @param entityID - the unique ID of the entity whose components are to be deleted.
            void removeComponents(guid_t entityID){
                for(auto &[componentTypeID, componentContainer]: containers) {
                    const auto &component = componentContainer.find(entityID);
                    const auto componentExistsInContainer = component != componentContainer.end();
                    if(componentExistsInContainer) {
                        componentContainer.erase(entityID);
                    }
                }
            }

            /// determines whether there is an existing component of the given type.
            template<typename C>
            bool hasComponentOfType() { return containers.count(typeid(C).hash_code()) != 0;}

            /// counts the amount of components of a given type currently in storage.
            template<typename C>
            int countComponentsOfType() { return containers[typeid(C).hash_code()].size();}
        private:
            using ComponentContainer = std::map<guid_t, std::unique_ptr<Component>>;

            /// A dictionary of component containers.
            /// Maps a container for a type X of component to its hashed typeid.
            std::map<size_t, ComponentContainer> containers;
            /// A reference to the engine instance to pass on to components.
            Game &engine;
        };
    }
}
