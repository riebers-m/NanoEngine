//
// Created by HP on 18.10.2024.
//

#ifndef MOVEMENT_HPP
#define MOVEMENT_HPP
#include "ECS/ECS.hpp"
#include "eventBus/EventBus.hpp"
#include "events/Collision.hpp"

namespace systems {
    class Movement : public ecs::System {
        using Logger = std::shared_ptr<engine::Logger>;

    private:
        Logger m_logger;

    public:
        explicit Movement(Logger);

        void subscribe_to_event(events::EventBus *event_bus);
        void on_collision(events::Collision &event);
        void on_enemy_hit_obstacle(ecs::Entity enemy, ecs::Entity obstacle);
        void update(float dt, std::uint16_t map_width, std::uint16_t map_height);
    };
} // namespace systems
#endif // MOVEMENT_HPP
