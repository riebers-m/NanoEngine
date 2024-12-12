//
// Created by HP on 02.11.2024.
//

#include "KeyboardMovement.hpp"

#include "components/Animation.hpp"
#include "components/KeyboardControlled.hpp"
#include "components/RigidBody.hpp"
#include "components/Sprite.hpp"
#include "eventBus/EventBus.hpp"

namespace systems {
    KeyboardMovement::KeyboardMovement(ecs::registry registry, Logger logger) :
        System{std::move(registry)}, m_logger{std::move(logger)} {
        // require_component<component::KeyboardControlled>();
        // require_component<component::Sprite>();
        // require_component<component::RigidBody>();
    }
    void KeyboardMovement::subscribe_to_event(events::EventBus *event_bus) {
        event_bus->subscribe<events::KeyPressedEvent>([this](events::KeyPressedEvent &event) { on_movement(event); });
    }
    void KeyboardMovement::on_movement(events::KeyPressedEvent &event) {
        auto const view = m_registry->view<component::KeyboardControlled, component::Sprite, component::RigidBody>();
        // for (auto const entity: get_entities()) {
        for (auto entity: view) {
            // auto const keyboard_controlled = entity.get_component<component::KeyboardControlled>();
            // auto &sprite = entity.get_component<component::Sprite>();
            // auto &rigid_body = entity.get_component<component::RigidBody>();

            auto const keyboard_controlled = m_registry->get<component::KeyboardControlled>(entity);
            auto [sprite, rigid_body] = m_registry->get<component::Sprite, component::RigidBody>(entity);

            switch (event.keycode) {
                case SDLK_UP:
                    rigid_body.velocity = keyboard_controlled.up_velocity;
                    sprite.src_rect.y = sprite.height * 0;
                    break;
                case SDLK_RIGHT:
                    rigid_body.velocity = keyboard_controlled.right_velocity;
                    sprite.src_rect.y = sprite.height * 1;
                    break;
                case SDLK_DOWN:
                    rigid_body.velocity = keyboard_controlled.down_velocity;
                    sprite.src_rect.y = sprite.height * 2;
                    break;
                case SDLK_LEFT:
                    rigid_body.velocity = keyboard_controlled.left_velocity;
                    sprite.src_rect.y = sprite.height * 3;
                    break;
            }
        }
    }
    void KeyboardMovement::update() {}
} // namespace systems
