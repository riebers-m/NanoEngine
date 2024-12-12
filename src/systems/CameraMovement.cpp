//
// Created by HP on 03.11.2024.
//

#include "CameraMovement.hpp"

#include <utility>

#include "components/CameraFollow.hpp"
#include "components/Transform.hpp"

namespace systems {
    CameraMovement::CameraMovement(ecs::registry registry, Logger logger) :
        System(std::move(registry)), m_logger{std::move(logger)} {
        // require_component<component::CameraFollow>();
        // require_component<component::Transform>();
    }
    void CameraMovement::update(SDL_Rect &camera, engine::Configuration const &config) {
        auto const view = m_registry->view<component::Transform, component::CameraFollow>();

        for (auto entity: view) {
            // for (auto const entity: get_entities()) {
            // auto const transform = entity.get_component<component::Transform>();
            auto const transform = m_registry->get<component::Transform>(entity);
            // Center entity on the screen and check min/max values to keep camera within map boundaries
            camera.x = static_cast<int>(glm::clamp(static_cast<double>(transform.position.x - (camera.w / 2)), 0.0,
                                                   static_cast<double>(config.map_width - camera.w)));
            camera.y = static_cast<int>(glm::clamp(static_cast<double>(transform.position.y - (camera.h / 2)), 0.0,
                                                   static_cast<double>(config.map_height - camera.h)));
        }
    }
} // namespace systems
