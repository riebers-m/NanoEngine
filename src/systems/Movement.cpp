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
    void Movement::update(float dt) {
        try {
            for (auto entity: get_entities()) {
                auto &transform = entity.get_component<component::Transform>();
                auto const rigid_body = entity.get_component<component::RigidBody>();

                transform.position += (rigid_body.velocity * dt);
            }
        } catch (std::exception const &e) {
            m_logger->error("Movement system could not update: {}", e.what());
        }
    }
} // namespace systems
