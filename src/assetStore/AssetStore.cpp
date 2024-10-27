//
// Created by HP on 21.10.2024.
//

#include "AssetStore.hpp"
#include <SDL.h>
#include <SDL_image.h>
#include <spdlog/spdlog.h>

void engine::AssetStore::TextureDeleter::operator()(SDL_Texture *t) const noexcept { SDL_DestroyTexture(t); }

engine::AssetStore::AssetStore(Logger logger) : m_logger{logger} {}

engine::AssetStore::~AssetStore() {}

void engine::AssetStore::clear_assets() { m_texture_store.clear(); }

void engine::AssetStore::add_texture(SDL_Renderer *renderer, std::string const &asset_id,
                                     std::filesystem::path const &file_path) {
    if (auto const surface = IMG_Load(file_path.string().c_str()); surface) {
        if (auto const texture = SDL_CreateTextureFromSurface(renderer, surface); texture) {
            SDL_FreeSurface(surface);
            if (auto [_, ok] = m_texture_store.emplace(asset_id, texture); !ok) {
                throw std::runtime_error(std::format("could not emplace texture {} in store", asset_id));
            }
            m_logger->debug("new texture added with id {}", asset_id);
            return;
        }
    }
    throw std::runtime_error(std::format("could not load image: {}", IMG_GetError()));
}

SDL_Texture *engine::AssetStore::get_texture(std::string const &asset_id) const {
    return m_texture_store.at(asset_id).get();
}
