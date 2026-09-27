#pragma once

#include "ecs.h"

namespace EisEngine{
    class Game;

    namespace ecs {
        /// A generic class for Components in Eis-Engine's Entity-Component-System
        /// Represents any form of data attributed to an entity.
        /// Components all derive directly or indirectly from this class.
        class Component {
            friend class ComponentManager;
            friend class Entity;
        public:
            /// Constructs the component and assigns it to the owner.
            explicit Component(Game &engine, guid_t owner = invalidID);
            /// Flags the component for deletion.
            virtual ~Component() = default;

            /// Returns the unique ID of the entity attached to this component.
            [[nodiscard]] guid_t GetOwner() const { return owner;}
            /// Determines whether a component has been flagged for deletion.
            [[nodiscard]] bool isDeleted() const { return deleted; }
            /// A pointer to the entity owning this component.
            Entity *entity() const;
        protected:
            /// A function made to encompass what needs to be done when a component is intentionally deleted.
            virtual void Invalidate() { deleted = true;}

            /// The unique ID of the entity this component is assigned to.
            guid_t owner;
            /// Determines whether the component is marked for deletion.
            bool deleted = false;
            /// A reference to the engine instance to access the entity manager.
            Game &engine;
        };
    }
}
