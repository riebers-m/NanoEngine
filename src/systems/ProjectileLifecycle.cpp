//
// Created by HP on 08.11.2024.
//

#include "ProjectileLifecycle.hpp"

#include "components/Animation.hpp"
#include "components/Projectile.hpp"

namespace systems {
    ProjectileLifecycle::ProjectileLifecycle(Logger logger) : m_logger{logger} {
        require_component<component::Projectile>();
    }
    void ProjectileLifecycle::update() {
        for (auto entity: get_entities()) {
            auto const projectile = entity.get_component<component::Projectile>();
            if (std::chrono::high_resolution_clock::now() - projectile.start_time > projectile.duration) {
                entity.kill();
            }
        }
    }
} // namespace systems
