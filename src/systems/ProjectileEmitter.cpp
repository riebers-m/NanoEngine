//
// Created by HP on 08.11.2024.
//

#include "ProjectileEmitter.hpp"
#include "components/ProjectileEmitter.hpp"

#include "components/BoxColider.hpp"
#include "components/CameraFollow.hpp"
#include "components/Projectile.hpp"
#include "components/RigidBody.hpp"
#include "components/Sprite.hpp"
#include "components/Transform.hpp"
#include "eventBus/EventBus.hpp"

namespace systems {
    ProjectileEmitter::ProjectileEmitter(Logger logger, ecs::Registry *registry) :
        m_logger{logger}, m_registry{registry} {
        require_component<component::ProjectileEmitter>();
        require_component<component::Transform>();
    }
    void ProjectileEmitter::update() {
        for (auto const entity: get_entities()) {
            auto &projectile_emitter = entity.get_component<component::ProjectileEmitter>();
            auto const transform = entity.get_component<component::Transform>();

            if (projectile_emitter.repeater == component::ProjectileEmitter::Repeater::manual) {
                continue;
            }
            if (std::chrono::high_resolution_clock::now() - projectile_emitter.lastEmissionTime >
                projectile_emitter.repeat_frequence) {
                auto projectile_position = transform.position;
                if (entity.has_component<component::Sprite>()) {
                    auto const sprite = entity.get_component<component::Sprite>();
                    projectile_position.x += (transform.scale.x * sprite.width / 2);
                    projectile_position.y += (transform.scale.y * sprite.height / 2);
                }
                auto projectile = m_registry->create_entity();
                projectile.group("projectiles");
                projectile.add_component<component::Transform>(projectile_position, transform.scale,
                                                               transform.rotation);
                projectile.add_component<component::RigidBody>(projectile_emitter.velocity);
                projectile.add_component<component::BoxColider>(4, 4);
                projectile.add_component<component::Sprite>("bullet-image", 4, 4, 4);
                projectile.add_component<component::Projectile>(
                        projectile_emitter.attitude, projectile_emitter.percentDamage, projectile_emitter.duration);

                projectile_emitter.lastEmissionTime = std::chrono::high_resolution_clock::now();
            }
        }
    }
    void ProjectileEmitter::subscribe_to_event(events::EventBus *event_bus) {
        event_bus->subscribe<events::KeyPressedEvent>([this](events::KeyPressedEvent &event) { on_keypress(event); });
    }
    void ProjectileEmitter::on_keypress(events::KeyPressedEvent &event) {
        if (event.keycode == SDLK_SPACE) {
            for (auto entity: get_entities()) {
                if (entity.has_component<component::CameraFollow>()) {
                    auto const projectile_emitter = entity.get_component<component::ProjectileEmitter>();
                    auto const transform = entity.get_component<component::Transform>();
                    auto const rigidbody = entity.get_component<component::RigidBody>();

                    auto projectile_position = transform.position;
                    if (entity.has_component<component::Sprite>()) {
                        auto const sprite = entity.get_component<component::Sprite>();
                        projectile_position.x += (transform.scale.x * sprite.width / 2);
                        projectile_position.y += (transform.scale.y * sprite.height / 2);
                    }
                    auto projectile = m_registry->create_entity();
                    projectile.group("projectiles");
                    projectile.add_component<component::Transform>(projectile_position, transform.scale,
                                                                   transform.rotation);

                    auto direction = glm::vec2{0.0, 0.0};
                    if (rigidbody.velocity.x > 0) {
                        direction.x += 1;
                    }
                    if (rigidbody.velocity.x < 0) {
                        direction.x -= 1;
                    }
                    if (rigidbody.velocity.y > 0) {
                        direction.y += 1;
                    }
                    if (rigidbody.velocity.y < 0) {
                        direction.y -= 1;
                    }

                    projectile.add_component<component::RigidBody>(projectile_emitter.velocity * direction);
                    projectile.add_component<component::BoxColider>(4, 4);
                    projectile.add_component<component::Sprite>("bullet-image", 4, 4, 4);
                    projectile.add_component<component::Projectile>(
                            projectile_emitter.attitude, projectile_emitter.percentDamage, projectile_emitter.duration);
                }
            }
        }
    }
} // namespace systems
