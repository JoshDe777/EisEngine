#include "GameObject.h"

namespace Ocean_Outlaws {
    Game* GameObject::engine = nullptr;

    //region constructors / destructors
    GameObject::GameObject(EisEngine::Game &_engine, const std::string& name, const std::string& tag) {
        entity = &_engine.entityManager.createEntity(name, tag, this);
        transform = entity->transform;
        if(!GameObject::engine)
            GameObject::engine = &_engine;
    }

    void GameObject::Invalidate(){
        engine->entityManager.deleteEntity(*entity);
    }

    void GameObject::Destroy(GameObject *gameObject) {
        gameObject->Invalidate();
        delete gameObject;
    }
    //endregion

    //region naming / tagging
    void GameObject::Rename(const std::string &newName) {
        entity->rename(newName);
    }

    void GameObject::SetTag(const std::string &newTag) {
        entity->setTag(newTag);
    }
    //endregion

    //region finding
    GameObject *GameObject::Find(const std::string &name) {
        if(!GameObject::engine){
            DEBUG_ERROR("[EisEngine error] - No engine pointer assigned to GameObject.")
            return nullptr;
        }

        auto* _entity = engine->entityManager.Find(name);
        if(!_entity){
            DEBUG_WARN("Could not find GameObject with the name " + name + ".")
            return nullptr;
        }

        auto linkingError = false;
        auto userData = _entity->user_data;
        if(!userData)
            linkingError = true;

        GameObject* gameObject = nullptr;

        // if user data not found, skip.
        if(!linkingError){
            // cast to GameObject pointer
            gameObject = static_cast<GameObject*>(userData);
            // catch undefined behaviour if cast fails + reset pointer to nullptr.
            try {
                if (!gameObject || gameObject->GetType() != typeid(GameObject)){
                    linkingError = true;
                    gameObject = nullptr;
                }
            } catch (const std::bad_typeid&) {
                DEBUG_WARN("Error comparing entity user data to game object.")
                linkingError = true;
                gameObject = nullptr;
            }
        }

        // send warning message if linking failed at any stage
        if(linkingError)
            DEBUG_ERROR("Game Object " + name + " initialized improperly (invalid user data pointer).")

        // gameObject is nullptr if linking failed, otherwise returns gameobject pointer.
        return gameObject;
    }

    std::vector<GameObject *> GameObject::FindAll(const std::string &name) {
        std::vector<GameObject*> result;

        if(!engine) {
            DEBUG_ERROR("[EisEngine error] - engine pointer not initialized.")
            return result;
        }

        for(auto _entity : engine->entityManager.FindAll(name)){
            if(!_entity){
                DEBUG_ERROR("Invalid entity with " + name + " found.")
                continue;
            }

            auto linkingError = false;
            auto userData = _entity->user_data;
            if(!userData)
                linkingError = true;

            GameObject* gameObject = nullptr;

            // if user data not found, skip.
            if(!linkingError){
                // cast to GameObject pointer
                gameObject = static_cast<GameObject*>(userData);
                // catch undefined behaviour if cast fails + reset pointer to nullptr.
                try {
                    if (!gameObject || gameObject->GetType() != typeid(GameObject)){
                        linkingError = true;
                        gameObject = nullptr;
                    }
                } catch (const std::bad_typeid&) {
                    linkingError = true;
                    gameObject = nullptr;
                }
            }

            // send warning message if linking failed at any stage
            if(linkingError) {
                DEBUG_ERROR("Game Object " + name + " initialized improperly (invalid user data pointer).")
                continue;
            }

            // if linking succeeded, add game object pointer to list.
            result.push_back(gameObject);
        }
        return result;
    }

    GameObject *GameObject::FindWithTag(const std::string &tag) {
        if(!GameObject::engine){
            DEBUG_ERROR("[EisEngine error] - No engine pointer assigned to GameObject.")
            return nullptr;
        }

        auto* _entity = engine->entityManager.FindWithTag(tag);
        if(!_entity){
            DEBUG_WARN("Could not find GameObject with the tag '" + tag + "'.")
            return nullptr;
        }

        auto linkingError = false;
        auto userData = _entity->user_data;
        if(!userData)
            linkingError = true;

        GameObject* gameObject = nullptr;

        // if user data not found, skip.
        if(!linkingError){
            // cast to GameObject pointer
            gameObject = static_cast<GameObject*>(userData);
            // catch undefined behaviour if cast fails + reset pointer to nullptr.
            try {
                if (!gameObject || gameObject->GetType() != typeid(GameObject)){
                    linkingError = true;
                    gameObject = nullptr;
                }
            } catch (const std::bad_typeid&) {
                linkingError = true;
                gameObject = nullptr;
            }
        }

        // send warning message if linking failed at any stage
        if(linkingError)
            DEBUG_ERROR("Game Object with tag '" + tag + "' initialized improperly (invalid user data pointer).")

        // gameObject is nullptr if linking failed, otherwise returns gameobject pointer.
        return gameObject;
    }

    std::vector<GameObject *> GameObject::FindAllWithTag(const std::string &tag) {
        std::vector<GameObject*> result;

        if(!engine) {
            DEBUG_ERROR("[EisEngine error] - engine pointer not initialized.")
            return result;
        }

        for(auto _entity : engine->entityManager.FindAllWithTag(tag)){
            if(!_entity){
                DEBUG_ERROR("Invalid entity with tag '" + tag + "' found.")
                continue;
            }

            auto linkingError = false;
            auto userData = _entity->user_data;
            if(!userData)
                linkingError = true;

            GameObject* gameObject = nullptr;

            // if user data not found, skip.
            if(!linkingError){
                // cast to GameObject pointer
                gameObject = static_cast<GameObject*>(userData);
                // catch undefined behaviour if cast fails + reset pointer to nullptr.
                try {
                    if (!gameObject || gameObject->GetType() != typeid(GameObject)){
                        linkingError = true;
                        gameObject = nullptr;
                    }
                } catch (const std::bad_typeid&) {
                    linkingError = true;
                    gameObject = nullptr;
                }
            }

            // send warning message if linking failed at any stage
            if(linkingError) {
                DEBUG_ERROR("Game Object with tag '" + tag + "' initialized improperly (invalid user data pointer).")
                continue;
            }

            // if linking succeeded, add game object pointer to list.
            result.push_back(gameObject);
        }
        return result;
    }
    //endregion
}
