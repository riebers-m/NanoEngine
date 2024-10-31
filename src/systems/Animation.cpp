//
// Created by HP on 31.10.2024.
//

#include "Animation.hpp"
#include "components/Animation.hpp"

#include "components/Sprite.hpp"

namespace systems {
    Animation::Animation(Logger logger) : m_logger(logger) {
        require_component<component::Sprite>();
        require_component<component::Animation>();
    }

    // TODO
    // Identifying animations by name
    // E.g. to have 3 frames for up_animation, 3 for down_animations, etc.
    void Animation::update(float dt) {
        for (auto entity: get_entities()) {
            auto &animation = entity.get_component<component::Animation>();
            auto &sprite = entity.get_component<component::Sprite>();

            animation.elapsed_time += dt;

            animation.current_frame =
                    static_cast<uint16_t>(animation.elapsed_time * animation.frame_speed_rate) % animation.num_frames;
            m_logger->debug("Animation current frame for entity{}: {}", entity.get_id(), animation.current_frame);

            sprite.src_rect.x = animation.current_frame * sprite.width;
        }
    }
} // namespace systems
