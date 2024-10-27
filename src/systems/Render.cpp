//
// Created by HP on 20.10.2024.
//

#include "systems/Render.hpp"

#include <SDL.h>

#include "assetStore/AssetStore.hpp"
#include "components/Sprite.hpp"
#include "components/Transform.hpp"

systems::RenderSystem::RenderSystem(Logger logger) : m_logger{logger} {
    require_component<component::Transform>();
    require_component<component::Sprite>();
}

void systems::RenderSystem::update(SDL_Renderer *renderer, engine::AssetStore const *asset_store) const {
    try {
        for (auto entity: get_entities()) {
            auto const transform = entity.get_component<component::Transform>();
            auto const sprite = entity.get_component<component::Sprite>();

            // Set source rect of original texture (sometime subrectangle if src is a sprite map
            SDL_Rect const src_rect = sprite.src_rect;
            // Set dest rect with x,y position to be rendered
            SDL_Rect const dest_rect = {static_cast<int>(transform.position.x), static_cast<int>(transform.position.y),
                                        static_cast<int>(sprite.width * transform.scale.x),
                                        static_cast<int>(sprite.height * transform.scale.y)

            };
            // NULL center point uses w/2 and h/2
            if (SDL_RenderCopyEx(renderer, asset_store->get_texture(sprite.asset_id), &src_rect, &dest_rect,
                                 transform.rotation, nullptr, SDL_FLIP_NONE) != 0) {
                throw std::runtime_error(std::format("rendering entity {} failed for asset id {}: {}", entity.get_id(),
                                                     sprite.asset_id, SDL_GetError()));
            }
        }
    } catch (std::exception const &e) {
        m_logger->error("render system error: {}", e.what());
    }
}
