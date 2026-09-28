#include "engine/ecs/EntityManager.h"
#include "engine/Game.h"

namespace EisEngine::ecs{
    EntityManager::EntityManager(
        ComponentManager &componentManager,
        Game &engine
    ) : componentManager(componentManager) { 
        // after update loops, erase entities marked for destruction.
        engine.onAfterUpdate.addListener([&](Game &) { purgeEntities();});
    }

    Entity *EntityManager::createEntity(const std::string &name, const std::string &tag, void* userData) {
        // create a new entity using an autoincrement id.
        int guid = entityCounter++;
        entities[guid] = std::make_unique<Entity>(guid, name, componentManager, tag, userData);
        return getEntity(guid);
    }

    Entity *EntityManager::getEntity(const guid_t guid) {
        // find the first entity with the given ID, or return a nullptr if none is found.
        if(entities.find(guid) != entities.end())
            return entities.at(guid).get();
        return nullptr;
    }

    Entity *EntityManager::Find(const std::string &name) {
        // find the first entity with the given name in the map.
        for (std::map<guid_t, std::unique_ptr<Entity>>::iterator i = entities.begin(); i != entities.end(); i++)
            if (i->second->name() == name)
                return entities.at(i->first).get();
        return nullptr;
    }

    std::vector<Entity *> EntityManager::FindAll(const std::string &name){
        // collect pointers to any entity with a name exactly matching the parameter.
        std::vector<Entity *> found;
        std::map<guid_t, std::unique_ptr<Entity>>::iterator i;
        for (i = entities.begin(); i != entities.end(); i++)
            if (i->second->name() == name)
                found.push_back(entities.at(i->first).get());
        return found;
    }

    Entity *EntityManager::FindWithTag(const std::string &tag) {
        // return the first entity with a tag matching the parameter, or a nullptr if none is found.
        for (std::map<guid_t, std::unique_ptr<Entity>>::iterator i = entities.begin(); i != entities.end(); i++)
            if (i->second->tag() == tag)
                return entities.at(i->first).get();
        return nullptr;
    }

    std::vector<Entity*> EntityManager::FindAllWithTag(const std::string &tag){
        // collect pointers to any entity with a tag exactly matching the parameter.
        std::vector<Entity*> found;
        std::map<guid_t, std::unique_ptr<Entity>>::iterator i;
        for (i = entities.begin(); i != entities.end(); i++)
            if (i->second->tag() == tag)
                found.push_back(entities.at(i->first).get());
        return found;
    }

    void EntityManager::deleteEntity(Entity &entity) {
        // mark an entity for deletion - internal mark, decouple components, and add to own deleteList.
        entity.deleted = true;
        entity.deleteAllComponents();
        deleteList.push_back(entity.guid());
    }

    void EntityManager::purgeEntities() {
        // early exit if no entity to delete.
        if (deleteList.size() == 0)
            return;

        // entities stored as unique_ptrs -> entities.erase auto-deletes them normally!
        for(auto &guid: deleteList)
            entities.erase(guid);
        // reset the deleteList after use :D
        deleteList.clear();
    }
}
