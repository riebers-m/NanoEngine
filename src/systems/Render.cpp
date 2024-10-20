//
// Created by HP on 20.10.2024.
//

#include "systems/Render.hpp"

#include <SDL.h>

#include "components/Sprite.hpp"
#include "components/Transform.hpp"

systems::RenderSystem::RenderSystem(Logger logger) : m_logger{logger} {
    require_component<component::Transform>();
    require_component<component::Sprite>();
}

void systems::RenderSystem::update(SDL_Renderer *renderer) const {
    try {
        for (auto entity: get_entities()) {
            auto const transform = entity.get_component<component::Transform>();
            auto const [width, height] = entity.get_component<component::Sprite>();

            SDL_Rect const rect = {static_cast<int>(transform.position.x), static_cast<int>(transform.position.y),
                                   width, height};
            if (SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255) != 0) {
                m_logger->error("could not set render draw color: {}", SDL_GetError());
            }
            if (SDL_RenderFillRect(renderer, &rect) != 0) {
                m_logger->error("could not draw fill rect for entity {}: {}", entity.get_id(), SDL_GetError());
            }
        }
    } catch (std::exception const &e) {
        m_logger->error("render system error: {}", e.what());
    }
}
