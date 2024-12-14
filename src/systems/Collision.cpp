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
        System{std::move(registry)}, m_logger{std::move(logger)} {}
    void Collision::update(events::EventBus *event_bus, ecs::Registry *registry) {
        auto view = m_registry->view<component::Transform, component::BoxCollider>();

        for (auto itr = view.begin(); itr != view.end(); ++itr) {
            auto const entity_a = *itr;
            auto const [transform_a, collider_a] =
                    m_registry->get<component::Transform, component::BoxCollider>(entity_a);

            for (auto j = itr; j != view.end(); ++j) {
                auto const entity_b = *j;
                if (entity_a == entity_b) {
                    continue;
                }
                auto const [transform_b, collider_b] =
                        m_registry->get<component::Transform, component::BoxCollider>(entity_b);

                if (engine::Commons::AABB_collision_check(
                            transform_a.position.x + collider_a.offset.x, transform_a.position.y + collider_a.offset.y,
                            collider_a.width, collider_a.height, transform_b.position.x + collider_b.offset.x,
                            transform_b.position.y + collider_b.offset.y, collider_b.width, collider_b.height)) {

                    event_bus->emit<events::Collision>(ecs::Entity{entity_a, registry},
                                                       ecs::Entity{entity_b, registry});
                }
            }
        }
    }
} // namespace systems
