//
// Created by HP on 02.11.2024.
//

#include "KeyboardMovement.hpp"
#include "eventBus/EventBus.hpp"

namespace systems {
    KeyboardMovement::KeyboardMovement(Logger logger) : m_logger{logger} {}
    void KeyboardMovement::subscribe_to_event(events::EventBus *event_bus) {
        event_bus->subscribe<events::KeyPressedEvent>([this](events::KeyPressedEvent &event) { on_movement(event); });
    }
    void KeyboardMovement::on_movement(events::KeyPressedEvent &event) {
        m_logger->debug("key {} pressed", static_cast<char>(event.keycode));
    }
    void KeyboardMovement::update() {}
} // namespace systems
