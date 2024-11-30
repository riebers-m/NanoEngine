//
// Created by HP on 20.10.2024.
//
#include "systems/Movement.hpp"

#include "components/RigidBody.hpp"
#include "components/Transform.hpp"

class Logger;
namespace systems {

    Movement::Movement(Logger logger) : m_logger{logger} {
        require_component<component::Transform>();
        require_component<component::RigidBody>();
    }
    void Movement::update(float dt, std::uint16_t map_width, std::uint16_t map_height) {
        try {
            for (auto entity: get_entities()) {
                auto &transform = entity.get_component<component::Transform>();
                auto const rigid_body = entity.get_component<component::RigidBody>();

                bool const is_outside_map = (transform.position.x < 0 || transform.position.x > map_width ||
                                             transform.position.y < 0 || transform.position.y > map_height);
                if (is_outside_map && !entity.has_tag("player")) {
                    entity.kill();
                    continue;
                }

                transform.position += (rigid_body.velocity * dt);
            }
        } catch (std::exception const &e) {
            // TODO
            // m_logger->error("Movement system could not update: {}", e.what());
        }
    }
} // namespace systems
