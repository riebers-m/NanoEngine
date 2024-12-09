//
// Created by HP on 09.12.2024.
//

#pragma once
#include "ECS/ECS.hpp"
#include "common/Logger.hpp"
#include "components/Animation.hpp"
#include "components/ProjectileEmitter.hpp"
#include "components/RigidBody.hpp"
#include "components/Script.hpp"
#include "components/Transform.hpp"

inline glm::vec2 get_entity_position(ecs::Entity entity) {
    if (entity.has_component<component::Transform>()) {
        return entity.get_component<component::Transform>().position;
    }
    return {};
}


inline glm::vec2 get_entity_velocity(ecs::Entity entity) {
    if (entity.has_component<component::RigidBody>()) {
        return entity.get_component<component::RigidBody>().velocity;
    }
    return {};
}

inline void set_entity_position(ecs::Entity entity, glm::vec2 const &new_position) {
    if (entity.has_component<component::Transform>()) {
        auto &transform = entity.get_component<component::Transform>();
        transform.position = new_position;
    }
}

inline void set_entity_velocity(ecs::Entity entity, glm::vec2 const &new_velocity) {
    if (entity.has_component<component::RigidBody>()) {
        auto &rigidbody = entity.get_component<component::RigidBody>();
        rigidbody.velocity = new_velocity;
    }
}

inline void set_entity_rotation(ecs::Entity entity, float new_angle) {
    if (entity.has_component<component::Transform>()) {
        auto &transfomr = entity.get_component<component::Transform>();
        transfomr.rotation = new_angle;
    }
}

inline void set_entity_animation_frame(ecs::Entity entity, int frame) {
    if (entity.has_component<component::Animation>()) {
        auto &animation = entity.get_component<component::Animation>();
        animation.current_frame = frame;
    }
}

inline void set_entity_projectile_velocity(ecs::Entity entity, glm::vec2 const &new_velocity) {
    if (entity.has_component<component::ProjectileEmitter>()) {
        auto &projectile_emitter = entity.get_component<component::ProjectileEmitter>();
        projectile_emitter.velocity = new_velocity;
    }
}

namespace systems {
    class Script : public ecs::System {
        using Logger = std::shared_ptr<engine::Logger>;

    private:
        Logger m_logger;

    public:
        explicit Script(Logger logger);

        void update(float dt, std::chrono::milliseconds const &elapsed_time) const;
        void create_lua_bindigs(sol::state &lua);
    };

} // namespace systems
