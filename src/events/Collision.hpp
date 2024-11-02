//
// Created by HP on 01.11.2024.
//

#ifndef COLLISION_EVENT_HPP
#define COLLISION_EVENT_HPP
#include "ECS/ECS.hpp"
#include "eventBus/Event.hpp"

namespace events {

    class Collision : public Event {
    public:
        ecs::Entity a;
        ecs::Entity b;

        explicit Collision(ecs::Entity a, ecs::Entity b);
    };

} // namespace events

#endif // COLLISION_EVENT_HPP
