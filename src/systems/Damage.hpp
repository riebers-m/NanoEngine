//
// Created by HP on 02.11.2024.
//

#ifndef DAMAGE_HPP
#define DAMAGE_HPP
#include "ECS/ECS.hpp"
#include "events/Collision.hpp"

namespace events {
    class EventBus;
}

namespace systems {

    class Damage : public ecs::System {
        using Logger = std::shared_ptr<spdlog::logger>;

    private:
        Logger m_logger;

    public:
        explicit Damage(Logger logger);

        void subscribe_to_event(events::EventBus *);
        void on_collision(events::Collision &event);
        void update();
    };

} // namespace systems

#endif // DAMAGE_HPP
