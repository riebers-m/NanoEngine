//
// Created by HP on 31.10.2024.
//

#ifndef COLLISION_HPP
#define COLLISION_HPP
#include "ECS/ECS.hpp"

namespace events {
    class EventBus;
}
namespace systems {

    class Collision : public ecs::System {
        using Logger = std::shared_ptr<spdlog::logger>;

    private:
        Logger m_logger;

    public:
        explicit Collision(Logger logger);
        void update(events::EventBus *);
    };

} // namespace systems

#endif // COLLISION_HPP
