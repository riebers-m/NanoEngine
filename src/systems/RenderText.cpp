//
// Created by HP on 21.11.2024.
//

#include "RenderText.hpp"
#include <functional>
#include "assetStore/AssetStore.hpp"
#include "components/TextLabel.hpp"

systems::RenderText::RenderText(ecs::registry registry, Logger logger) :
    System{std::move(registry)}, m_logger{std::move(logger)} { /* require_component<component::TextLabel>(); */
}

void systems::RenderText::update(SDL_Renderer *renderer, engine::AssetStore *asset_store, SDL_Rect const &camera) {
    auto const view = m_registry->view<component::TextLabel>();
    // for (auto entity: get_entities()) {
    for (auto entity_id: view) {
        // auto const text_label = entity.get_component<component::TextLabel>();
        auto const text_label = m_registry->get<component::TextLabel>(entity_id);

        if (std::unique_ptr<SDL_Surface, std::function<void(SDL_Surface * s)>> surface{
                    TTF_RenderText_Blended(asset_store->get_font(text_label.asset_id), text_label.text.c_str(),
                                           text_label.color),
                    [](SDL_Surface *s) { SDL_FreeSurface(s); }};
            surface != nullptr) {

            if (std::unique_ptr<SDL_Texture, std::function<void(SDL_Texture *)>> texture{
                        SDL_CreateTextureFromSurface(renderer, surface.get()),
                        [](SDL_Texture *t) { SDL_DestroyTexture(t); }};
                texture != nullptr) {
                int label_width = 0;
                int label_height = 0;

                if (SDL_QueryTexture(texture.get(), nullptr, nullptr, &label_width, &label_height) != 0) {
                    m_logger->error("Query texture {} failed: {}", text_label.asset_id, SDL_GetError());
                    return;
                }

                SDL_Rect dest = {
                        static_cast<int>(text_label.position.x -
                                         (text_label.world_position == component::WorldPosition::fixed ? 0 : camera.x)),
                        static_cast<int>(text_label.position.y -
                                         (text_label.world_position == component::WorldPosition::fixed ? 0 : camera.y)),
                        label_width, label_height};
                SDL_RenderCopy(renderer, texture.get(), nullptr, &dest);
            }
        }
    }
}
