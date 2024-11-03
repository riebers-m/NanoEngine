//
// Created by HP on 03.11.2024.
//

#include "CameraMovement.hpp"

#include "components/Animation.hpp"
#include "components/CameraFollow.hpp"
#include "components/Transform.hpp"

namespace systems {
    CameraMovement::CameraMovement(Logger logger) : m_logger{logger} {
        require_component<component::CameraFollow>();
        require_component<component::Transform>();
    }
    void CameraMovement::update(SDL_Rect &camera, engine::Configuration const &config) {
        for (auto const entity: get_entities()) {
            auto const transform = entity.get_component<component::Transform>();

            // Center entity on the screen and check min/max values to keep camera within map boundaries
            camera.x = static_cast<int>(glm::clamp(static_cast<double>(transform.position.x - (camera.w / 2)), 0.0,
                                                   static_cast<double>(config.map_width - camera.w)));
            camera.y = static_cast<int>(glm::clamp(static_cast<double>(transform.position.y - (camera.h / 2)), 0.0,
                                                   static_cast<double>(config.map_height - camera.h)));

            m_logger->debug("changed camera position to {}, {}", camera.x, camera.y);
        }
    }
} // namespace systems
