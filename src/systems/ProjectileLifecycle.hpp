//
// Created by HP on 08.11.2024.
//

#pragma once
#include "ECS/ECS.hpp"

namespace systems {

    class ProjectileLifecycle : public ecs::System {
        using Logger = std::shared_ptr<engine::Logger>;

    private:
        Logger m_logger;

    public:
        explicit ProjectileLifecycle(Logger logger);

        void update();
    };

} // namespace systems
