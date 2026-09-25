#pragma once

#include "EisEngine.h"

namespace Ocean_Outlaws {
    /// \n Mantle class for entities with improved usability.
    class GameObject {
    public:
        //region constructors & destructors
        /// \n Creates a new game object.
        /// @param _engine - Game&: A reference to the engine instance to access entity and component management.
        /// @param name - std::string: The name given to the GameObject. Defaults to 'GameObject'
        /// @param tag - std::string: The tag given to the GameObject. Defaults to 'untagged'.
        explicit GameObject(Game &_engine, const std::string& name = "GameObject", const std::string &tag = "untagged");

        /// \n Removes a GameObject from the world.
        static void Destroy(GameObject* gameObject);
        //endregion

        //region component management
        /// \n Adds a component of a given type to the game object.
        /// @return Component*: A pointer to the newly created component.
        template<typename C, typename ...Args>
        C* AddComponent(Args ...args) { return &entity->AddComponent<C>(args...);}

        /// \n Gets a component of the given type assigned to the game object.
        /// @return Component*:  A pointer to the desired component. Returns a nullptr if no component is found.
        template<typename C>
        C* GetComponent() { return entity->GetComponent<C>();}
        //endregion

        //region naming and tagging
        /// \n Gets the GameObject's name.
        std::string name() { return entity->name();}

        /// \n Gets the GameObject's tag.
        std::string tag() { return entity->tag();}

        /// \n Renames the GameObject. Not necessarily unique.
        void Rename(const std::string& newName);

        /// \n Overwrites the current GameObject's tag.
        void SetTag(const std::string& newTag);
        //endregion

        //region finding game objects.
        /// \n Find a game object with the given name.
        /// @param name - std::string: the non-unique name of the sought after GameObject.
        /// @return GameObject*: A pointer to a game object with the given name.
        /// \n Returns a nullptr if none is found.
        static GameObject* Find(const std::string& name);

        /// \n Find all GameObjects with the given name.
        /// @param name - std::string: The non-unique name of the sought after GameObjects.
        /// @return std::vector&lt;GameObject *>: a list of pointers to GameObjects with the given name.
        /// \n Returns an empty list if none is found.
        static std::vector<GameObject *> FindAll(const std::string& name);

        /// \n Find a GameObject assigned to the tag.
        /// @return GameObject*: A pointer to the first GameObject found with the given tag.
        /// \n Returns a nullptr if none is found.
        static GameObject* FindWithTag(const std::string& tag);

        /// \n Find all GameObjects assigned to the tag.
        /// @return std::vector&lt;GameObject*>: A list of pointers to GameObjects with the given tag.
        static std::vector<GameObject *> FindAllWithTag(const std::string& tag);

        template<typename C>
        static C* FindObjectOfType(){
            if(!GameObject::engine){
                DEBUG_ERROR("[EisEngine error] - No engine pointer assigned to GameObject.")
                return nullptr;
            }
            return engine->entityManager.FindEntityOfType<C>();
        }
        //endregion

        /// \n the transform assigned to the game object.
        Transform* transform;

        operator std::string(){ return "<GameObject::" + name() + ">";}
    protected:
        /// \n the EisEngine ecs entity linked with the game object.
        Entity* entity;
        /// \n a static pointer to the active engine instance, to access the entity management system.
        static Game* engine;

        /// \n This function is called once per frame.
        virtual void Update() { }

        /// \n This function is called when a game object is intentionally deleted.
        virtual void Invalidate();

        [[nodiscard]] virtual const std::type_info& GetType() const {
            return typeid(GameObject);
        }
    };
}
