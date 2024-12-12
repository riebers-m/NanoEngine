//
// Created by HP on 08.11.2024.
//
#include "ECS/ECS.hpp"
#include "common/Logger.hpp"
#pragma once


namespace systems {

    class ProjectileLifecycle : public ecs::System {
        using Logger = std::shared_ptr<engine::Logger>;

    private:
        Logger m_logger;

    public:
        explicit ProjectileLifecycle(ecs::registry registry, Logger logger);

        void update(ecs::Registry *);
    };

} // namespace systems
