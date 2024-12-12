//
// Created by HP on 08.11.2024.
//

#include "ProjectileLifecycle.hpp"
#include "components/Projectile.hpp"

namespace systems {
    ProjectileLifecycle::ProjectileLifecycle(ecs::registry registry, Logger logger) :
        System{std::move(registry)}, m_logger{logger} {
        // require_component<component::Projectile>();
    }
    void ProjectileLifecycle::update(ecs::Registry *registry) {
        // for (auto entity: get_entities()) {
        // auto const projectile = entity.get_component<component::Projectile>();
        auto const view = m_registry->view<component::Projectile>();
        for (auto entity_id: view) {
            auto const projectile = m_registry->get<component::Projectile>(entity_id);
            ecs::Entity entity{entity_id, registry};
            if (std::chrono::high_resolution_clock::now() - projectile.start_time > projectile.duration) {
                entity.kill();
            }
        }
    }
} // namespace systems
