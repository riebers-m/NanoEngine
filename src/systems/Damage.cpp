//
// Created by HP on 02.11.2024.
//

#include "Damage.hpp"

#include "components/BoxColider.hpp"
#include "components/Health.hpp"
#include "components/Projectile.hpp"
#include "eventBus/EventBus.hpp"

namespace systems {
    Damage::Damage(Logger logger) : m_logger{logger} { require_component<component::BoxColider>(); }
    void Damage::subscribe_to_event(events::EventBus *event_bus) {
        event_bus->subscribe<events::Collision>([this](events::Collision &event) { on_collision(event); });
    }
    void Damage::on_collision(events::Collision &event) {
        ecs::Entity a = event.a;
        ecs::Entity b = event.b;
        if (a.has_group("projectiles") && b.has_tag("player")) {
            on_projectile_hits_player(a, b);
        }
        if (b.has_group("projectiles") && a.has_tag("player")) {
            on_projectile_hits_player(b, a);
        }
        if (a.has_group("projectiles") && b.has_group("enemies")) {
            on_projectile_hits_enemy(a, b);
        }
        if (b.has_group("projectiles") && a.has_group("enemies")) {
            on_projectile_hits_enemy(b, a);
        }
    }
    void Damage::update() {}

    void Damage::on_projectile_hits_player(ecs::Entity projectile, ecs::Entity player) {
        auto projectile_component = projectile.get_component<component::Projectile>();

        if (projectile_component.attitude == component::ProjectileEmitter::Attitude::enemy) {
            auto &health_component = player.get_component<component::Health>();
            health_component.health_percentage -= projectile_component.hit_percent_damage;
            if (health_component.health_percentage <= 0) {
                player.kill();
            }
            projectile.kill();
        }
    }

    void Damage::on_projectile_hits_enemy(ecs::Entity projectile, ecs::Entity enemy) {
        auto projectile_component = projectile.get_component<component::Projectile>();

        if (projectile_component.attitude == component::ProjectileEmitter::Attitude::friendly) {
            auto &health_component = enemy.get_component<component::Health>();
            health_component.health_percentage -= projectile_component.hit_percent_damage;
            if (health_component.health_percentage <= 0) {
                enemy.kill();
            }
            projectile.kill();
        }
    }
} // namespace systems
