//
// Created by HP on 21.11.2024.
//

#include "HealthBar.hpp"

#include <SDL_ttf.h>

#include "assetStore/AssetStore.hpp"
#include "components/Health.hpp"
#include "components/Sprite.hpp"
#include "components/Transform.hpp"

systems::HealthBar::HealthBar(Logger logger) : m_logger{logger} {
    require_component<component::Health>();
    require_component<component::Transform>();
    require_component<component::Sprite>();
}
void systems::HealthBar::update(SDL_Renderer *renderer, engine::AssetStore *asset_store, SDL_Rect const &camera) {
    for (auto entity: get_entities()) {
        auto const health = entity.get_component<component::Health>();
        auto const transform = entity.get_component<component::Transform>();

        SDL_Color color{0, 255, 0};
        if (health.health_percentage <= 30) {
            color = {255, 0, 0};
        } else if (health.health_percentage <= 70) {
            color = {255, 165, 0};
        }

        if (SDL_Surface *surface = TTF_RenderText_Blended(asset_store->get_font("charriot-text"),
                                                          std::format("{}%", health.health_percentage).c_str(), color);
            surface != nullptr) {
            if (SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface); texture != nullptr) {
                int label_width = 0;
                int label_height = 0;

                if (SDL_QueryTexture(texture, nullptr, nullptr, &label_width, &label_height) < 0) {
                    m_logger->error("query texture failed: {}", SDL_GetError());
                    SDL_FreeSurface(surface);
                    return;
                }
                auto const sprite = entity.get_component<component::Sprite>();
                SDL_Rect dest = {static_cast<int>(transform.position.x - camera.x + sprite.width / 2),
                                 static_cast<int>(transform.position.y - camera.y - 24), label_width, label_height};
                SDL_RenderCopy(renderer, texture, nullptr, &dest);

                SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);
                SDL_Rect line = {static_cast<int>(transform.position.x - camera.x + sprite.width / 2),
                                 static_cast<int>(transform.position.y - camera.y - 10),
                                 static_cast<int>(sprite.width * health.health_percentage / 100), 3};
                SDL_RenderFillRect(renderer, &line);
            }
            SDL_FreeSurface(surface);
        }
    }
}
