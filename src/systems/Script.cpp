//
// Created by HP on 09.12.2024.
//

#include "Script.hpp"
#include "components/Script.hpp"

#include <sol/state.hpp>

systems::Script::Script(Logger logger) : m_logger{std::move(logger)} { require_component<component::Script>(); }
void systems::Script::update(float dt, std::chrono::milliseconds const &elapsed_time) const {
    for (auto entity: get_entities()) {
        auto const script = entity.get_component<component::Script>();
        script.func(entity, dt, static_cast<std::uint32_t>(elapsed_time.count()));
    }
}

void systems::Script::create_lua_bindigs(sol::state &lua) {
    // create the entity user type, so lua knows what an entity is
    lua.new_usertype<ecs::Entity>("entity", "get_id", &ecs::Entity::get_id, "destroy", &ecs::Entity::kill, "has_tag",
                                  &ecs::Entity::has_tag, "has_group", &ecs::Entity::has_group);
    lua.new_usertype<glm::vec2>("vec2", sol::constructors<glm::vec2(float, float), glm::vec2()>(), "x", &glm::vec2::x,
                                "y", &glm::vec2::y);
    // create all bindings between c++ and lua
    lua.set_function("set_position", set_Entityposition);
    lua.set_function("get_position", get_Entityposition);
    lua.set_function("get_velocity", get_Entityvelocity);
    lua.set_function("set_velocity", set_Entityvelocity);
    lua.set_function("set_rotation", set_Entityrotation);
    lua.set_function("set_projectile_velocity", set_Entityprojectile_velocity);
    lua.set_function("set_animation_frame", set_Entityanimation_frame);
}
