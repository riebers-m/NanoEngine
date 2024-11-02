//
// Created by HP on 02.11.2024.
//

#include "Damage.hpp"

#include "components/BoxColider.hpp"
#include "eventBus/EventBus.hpp"

namespace systems {
    Damage::Damage(Logger logger) : m_logger{logger} { require_component<component::BoxColider>(); }
    void Damage::subscribe_to_event(events::EventBus *event_bus) {
        event_bus->subscribe<events::Collision>([this](events::Collision &event) { on_collision(event); });
    }
    void Damage::on_collision(events::Collision &event) {
        m_logger->debug("damage system received collision event between {} <-> {}.", event.a.get_id(),
                        event.b.get_id());
        event.a.kill();
        event.b.kill();
    }
    void Damage::update() {}
} // namespace systems
