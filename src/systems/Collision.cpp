//
// Created by HP on 31.10.2024.
//

#include "Collision.hpp"
#include "common/Commons.hpp"
#include "components/BoxCollider.hpp"
#include "components/Transform.hpp"
#include "eventBus/EventBus.hpp"
#include "events/Collision.hpp"

namespace systems {
    Collision::Collision(Logger logger) : m_logger{logger} {
        require_component<component::Transform>();
        require_component<component::BoxCollider>();
    }
    void Collision::update(events::EventBus *event_bus) {

        auto const entities = get_entities();

        // to prevent double checks only check against entities infront of current entities in the vector
        for (auto itr = entities.begin(); itr != entities.end(); ++itr) {
            ecs::Entity a = *itr;
            auto const transform_a = a.get_component<component::Transform>();
            auto const collider_a = a.get_component<component::BoxCollider>();

            for (auto j = itr + 1; j != entities.end(); ++j) {
                ecs::Entity b = *j;
                if (a == b) {
                    continue;
                }
                auto const transform_b = b.get_component<component::Transform>();
                auto const collider_b = b.get_component<component::BoxCollider>();

                if (engine::Commons::AABB_collision_check(
                            transform_a.position.x + collider_a.offset.x, transform_a.position.y + collider_a.offset.y,
                            collider_a.width, collider_a.height, transform_b.position.x + collider_b.offset.x,
                            transform_b.position.y + collider_b.offset.y, collider_b.width, collider_b.height)) {

                    event_bus->emit<events::Collision>(a, b);
                }
            }
        }
    }
} // namespace systems
