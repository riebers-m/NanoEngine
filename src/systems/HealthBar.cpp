//
// Created by HP on 21.11.2024.
//

#include "HealthBar.hpp"

#include <SDL_ttf.h>

#include <functional>
#include "assetStore/AssetStore.hpp"
#include "components/Health.hpp"
#include "components/Sprite.hpp"
#include "components/Transform.hpp"

systems::HealthBar::HealthBar(ecs::registry registry, Logger logger) :
    System{std::move(registry)}, m_logger{std::move(logger)} {
    // require_component<component::Health>();
    // require_component<component::Transform>();
    // require_component<component::Sprite>();
}
void systems::HealthBar::update(SDL_Renderer *renderer, engine::AssetStore *asset_store, SDL_Rect const &camera) {
    auto const view = m_registry->view<component::Health, component::Transform, component::Sprite>();
    // for (auto entity: get_entities()) {
    // auto const health = entity.get_component<component::Health>();
    // auto const transform = entity.get_component<component::Transform>();
    for (auto entity: view) {
        auto const [health, transform] = m_registry->get<component::Health, component::Transform>(entity);

        SDL_Color color{0, 255, 0};
        if (health.health_percentage <= 30) {
            color = {255, 0, 0};
        } else if (health.health_percentage <= 70) {
            color = {255, 165, 0};
        }

        if (std::unique_ptr<SDL_Surface, std::function<void(SDL_Surface * s)>> surface{
                    TTF_RenderText_Blended(asset_store->get_font("pico8-font-5"),
                                           std::format("{}%", health.health_percentage).c_str(), color),
                    [](SDL_Surface *s) { SDL_FreeSurface(s); }};
            surface != nullptr) {
            if (std::unique_ptr<SDL_Texture, std::function<void(SDL_Texture *)>> texture{
                        SDL_CreateTextureFromSurface(renderer, surface.get()),
                        [](SDL_Texture *t) { SDL_DestroyTexture(t); }};
                texture != nullptr) {

                int label_width = 0;
                int label_height = 0;

                if (SDL_QueryTexture(texture.get(), nullptr, nullptr, &label_width, &label_height) < 0) {
                    m_logger->error("query texture failed: {}", SDL_GetError());
                    return;
                }
                // auto const sprite = entity.get_component<component::Sprite>();
                auto const sprite = m_registry->get<component::Sprite>(entity);
                SDL_Rect dest = {static_cast<int>(transform.position.x - camera.x + sprite.width / 2),
                                 static_cast<int>(transform.position.y - camera.y - 24), label_width, label_height};
                SDL_RenderCopy(renderer, texture.get(), nullptr, &dest);

                SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);
                SDL_Rect line = {static_cast<int>(transform.position.x - camera.x + sprite.width / 2),
                                 static_cast<int>(transform.position.y - camera.y - 10),
                                 static_cast<int>(sprite.width * health.health_percentage / 100), 3};
                SDL_RenderFillRect(renderer, &line);
            }
        }
    }
}
