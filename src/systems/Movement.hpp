//
// Created by HP on 18.10.2024.
//

#ifndef MOVEMENT_HPP
#define MOVEMENT_HPP
#include "ECS/ECS.hpp"

namespace systems {
    class Movement : public ecs::System {
        using Logger = std::shared_ptr<engine::Logger>;

    private:
        Logger m_logger;

    public:
        explicit Movement(Logger);

        void update(float dt, std::uint16_t map_width, std::uint16_t map_height);
    };
} // namespace systems
#endif // MOVEMENT_HPP
