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
    KeyboardMovement::KeyboardMovement(Logger logger) : m_logger{logger} {
        require_component<component::KeyboardControlled>();
        require_component<component::Sprite>();
        require_component<component::RigidBody>();
    }
    void KeyboardMovement::subscribe_to_event(events::EventBus *event_bus) {
        event_bus->subscribe<events::KeyPressedEvent>([this](events::KeyPressedEvent &event) { on_movement(event); });
    }
    void KeyboardMovement::on_movement(events::KeyPressedEvent &event) {
        for (auto const entity: get_entities()) {
            auto const keyboard_controlled = entity.get_component<component::KeyboardControlled>();
            auto &sprite = entity.get_component<component::Sprite>();
            auto &rigid_body = entity.get_component<component::RigidBody>();

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
