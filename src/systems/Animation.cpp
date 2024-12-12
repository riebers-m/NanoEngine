//
// Created by HP on 31.10.2024.
//

#include "Animation.hpp"

#include <utility>
#include "components/Animation.hpp"

#include "components/Sprite.hpp"

namespace systems {
    Animation::Animation(ecs::registry registry, Logger logger) :
        System{std::move(registry)}, m_logger(std::move(logger)) {}

    // TODO
    // Identifying animations by name
    // E.g. to have 3 frames for up_animation, 3 for down_animations, etc.
    void Animation::update(float dt) {
        auto view = m_registry->view<component::Animation, component::Sprite>();
        for (auto entity: view) {
            // for (auto entity: get_entities()) {
            // auto &animation = entity.get_component<component::Animation>();
            // auto &sprite = entity.get_component<component::Sprite>();

            auto [animation, sprite] = view.get<component::Animation, component::Sprite>(entity);
            animation.elapsed_time += dt;

            animation.current_frame =
                    static_cast<uint16_t>(animation.elapsed_time * animation.frame_speed_rate) % animation.num_frames;

            sprite.src_rect.x = animation.current_frame * sprite.width;
        }
    }
} // namespace systems
