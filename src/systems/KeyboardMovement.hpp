//
// Created by HP on 02.11.2024.
//

#ifndef KEYBOARDMOVEMENT_HPP
#define KEYBOARDMOVEMENT_HPP
#include "ECS/ECS.hpp"
#include "events/KeyPressedEvent.hpp"

namespace events {
    class EventBus;
}

namespace systems {

    class KeyboardMovement : public ecs::System {
        using Logger = std::shared_ptr<engine::Logger>;

    private:
        Logger m_logger;

    public:
        explicit KeyboardMovement(Logger logger);

        void subscribe_to_event(events::EventBus *);
        void on_movement(events::KeyPressedEvent &event);
        void update();
    };

} // namespace systems

#endif // KEYBOARDMOVEMENT_HPP
