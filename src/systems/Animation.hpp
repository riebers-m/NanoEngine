//
// Created by HP on 31.10.2024.
//

#ifndef ANIMATIONSYSTEM_HPP
#define ANIMATIONSYSTEM_HPP
#include "ECS/ECS.hpp"

namespace systems {

    class Animation : public ecs::System {
        using Logger = std::shared_ptr<engine::Logger>;

    private:
        Logger m_logger;

    public:
        explicit Animation(ecs::registry registry, Logger logger);

        void update(float);
    };

} // namespace systems

#endif // ANIMATIONSYSTEM_HPP
