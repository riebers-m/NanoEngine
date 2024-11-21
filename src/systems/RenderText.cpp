//
// Created by HP on 21.11.2024.
//

#include "RenderText.hpp"
#include "assetStore/AssetStore.hpp"
#include "components/TextLabel.hpp"

systems::RenderText::RenderText(Logger logger) : m_logger{logger} { require_component<component::TextLabel>(); }

void systems::RenderText::update(SDL_Renderer *renderer, engine::AssetStore *asset_store, SDL_Rect const &camera) {
    for (auto entity: get_entities()) {
        auto const text_label = entity.get_component<component::TextLabel>();
        if (SDL_Surface *surface = TTF_RenderText_Blended(asset_store->get_font(text_label.asset_id),
                                                          text_label.text.c_str(), text_label.color);
            surface != nullptr) {
            if (SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface); texture != nullptr) {

                int label_width = 0;
                int label_height = 0;

                if (SDL_QueryTexture(texture, nullptr, nullptr, &label_width, &label_height) != 0) {
                    m_logger->error("query texture {} failed: {}", text_label.asset_id, SDL_GetError());
                    return;
                }

                SDL_Rect dest = {
                        static_cast<int>(text_label.position.x -
                                         (text_label.world_position == component::WorldPosition::fixed ? 0 : camera.x)),
                        static_cast<int>(text_label.position.y -
                                         (text_label.world_position == component::WorldPosition::fixed ? 0 : camera.y)),
                        label_width, label_height};
                SDL_RenderCopy(renderer, texture, nullptr, &dest);
            }
            SDL_FreeSurface(surface);
        }
    }
}
