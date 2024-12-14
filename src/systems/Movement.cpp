//
// Created by HP on 20.10.2024.
//
#include <utility>

#include "systems/Movement.hpp"

#include "components/RigidBody.hpp"
#include "components/Sprite.hpp"
#include "components/Transform.hpp"

class Logger;
namespace systems {

    Movement::Movement(ecs::registry registry, Logger logger) :
        System{std::move(registry)}, m_logger{std::move(logger)} {}

    void Movement::subscribe_to_event(events::EventBus *event_bus) {
        event_bus->subscribe<events::Collision>([this](events::Collision &event) { on_collision(event); });
    }
    void Movement::on_collision(events::Collision &event) {
        ecs::Entity const a = event.a;
        ecs::Entity const b = event.b;

        if (a.has_group("enemies") && b.has_group("obstacles")) {
            on_enemy_hit_obstacle(a, b);
        }

        if (a.has_group("obstacles") && b.has_group("enemies")) {
            on_enemy_hit_obstacle(b, a);
        }
    }
    void Movement::on_enemy_hit_obstacle(ecs::Entity enemy, ecs::Entity obstacle) {
        if (enemy.has_component<component::RigidBody>() && enemy.has_component<component::Sprite>()) {
            auto &rigid_body = enemy.get_component<component::RigidBody>();
            auto &sprite = enemy.get_component<component::Sprite>();

            if (rigid_body.velocity.x != 0) {
                rigid_body.velocity.x *= -1;
                sprite.flip = (sprite.flip == SDL_FLIP_NONE ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);
            }
            if (rigid_body.velocity.y != 0) {
                rigid_body.velocity.y *= -1;
                sprite.flip = (sprite.flip == SDL_FLIP_NONE ? SDL_FLIP_VERTICAL : SDL_FLIP_NONE);
            }
        }
    }


    void Movement::update(float dt, std::uint16_t map_width, std::uint16_t map_height, ecs::Registry *registry) {
        try {
            auto const view = m_registry->view<component::Transform, component::RigidBody, component::Sprite>();
            for (auto entity_id: view) {
                auto &transform = m_registry->get<component::Transform>(entity_id);
                auto const [rigid_body, sprite] = m_registry->get<component::RigidBody, component::Sprite>(entity_id);

                bool const is_outside_map = (transform.position.x < 0 || transform.position.x > map_width ||
                                             transform.position.y < 0 || transform.position.y > map_height);
                auto entity = ecs::Entity{entity_id, registry};
                if (is_outside_map && !entity.has_tag("player")) {
                    entity.kill();
                    continue;
                }
                transform.position += (rigid_body.velocity * dt);

                if (entity.has_tag("player")) {
                    transform.position.x = (transform.position.x < 0) ? 0 : transform.position.x;
                    transform.position.x =
                            (transform.position.x + static_cast<float>(sprite.width) >= static_cast<float>(map_width))
                                    ? static_cast<float>(map_width - sprite.width)
                                    : transform.position.x;
                    transform.position.y = (transform.position.y < 0) ? 0 : transform.position.y;
                    transform.position.y =
                            (transform.position.y + static_cast<float>(sprite.height) >= static_cast<float>(map_height))
                                    ? static_cast<float>(map_height - sprite.height)
                                    : transform.position.y;
                }
            }
        } catch (std::exception const &e) {
            m_logger->error("Movement system could not update: {}", e.what());
        }
    }
} // namespace systems
