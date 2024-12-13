//
// Created by HP on 31.10.2024.
//

#include "Collision.hpp"

#include <utility>
#include "common/Commons.hpp"
#include "components/BoxCollider.hpp"
#include "components/Transform.hpp"
#include "eventBus/EventBus.hpp"
#include "events/Collision.hpp"

namespace systems {
    Collision::Collision(ecs::registry registry, Logger logger) :
        System{std::move(registry)}, m_logger{std::move(logger)} {
        // require_component<component::Transform>();
        // require_component<component::BoxCollider>();
    }
    void Collision::update(events::EventBus *event_bus, ecs::Registry *registry) {
        auto view = m_registry->view<component::Transform, component::BoxCollider>();
        // auto const entities = get_entities();

        for (auto entity_id: view) {
            auto const transform = m_registry->get<component::Transform>(entity_id);
            m_logger->debug("pos: {},{}", transform.position.x, transform.position.y);
        }
        // TODO: Exception occuring after refactoring with entt framework
        // to prevent double checks only check against entities infront of current entities in the vector
        for (auto itr = view.begin(); itr != view.end(); ++itr) {
            auto const entity_a = *itr;
            auto const [transform_a, collider_a] =
                    m_registry->get<component::Transform, component::BoxCollider>(entity_a);
            // auto const transform_a = a.get_component<component::Transform>();
            // auto const collider_a = a.get_component<component::BoxCollider>();
            for (auto j = itr; j != view.end(); ++j) {
                auto const entity_b = *j;
                if (entity_a == entity_b) {
                    continue;
                }
                // auto const transform_b = b.get_component<component::Transform>();
                // auto const collider_b = b.get_component<component::BoxCollider>();
                auto const [transform_b, collider_b] =
                        m_registry->get<component::Transform, component::BoxCollider>(entity_b);

                if (engine::Commons::AABB_collision_check(
                            transform_a.position.x + collider_a.offset.x, transform_a.position.y + collider_a.offset.y,
                            collider_a.width, collider_a.height, transform_b.position.x + collider_b.offset.x,
                            transform_b.position.y + collider_b.offset.y, collider_b.width, collider_b.height)) {

                    // event_bus->emit<events::Collision>(entity_a, entity_b);
                    event_bus->emit<events::Collision>(ecs::Entity{entity_a, registry},
                                                       ecs::Entity{entity_b, registry});
                }
            }
        }
    }
} // namespace systems
