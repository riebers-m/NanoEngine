//
// Created by HP on 20.10.2024.
//

#ifndef SPRITE_HPP
#define SPRITE_HPP
#include <SDL.h>
#include <string>
#include <utility>
#include "Common.hpp"

namespace component {
    struct Sprite {
        std::string asset_id;
        int width{};
        int height{};
        int z_index{};
        SDL_Rect src_rect{};
        WorldPosition position{WorldPosition::free};

        // default values for all arguments are needed because you cant default construct this class anymore (needed in
        // the registry.add_component function)
        explicit Sprite(std::string asset_id_ = "", int width_ = 0, int height_ = 0, int z_index = 0,
                        WorldPosition position = WorldPosition::free, int src_rect_x_ = 0, int src_rect_y_ = 0) :
            asset_id(std::move(asset_id_)), width(width_), height(height_), z_index{z_index},
            src_rect{src_rect_x_, src_rect_y_, width_, height_}, position{position} {}
    };
} // namespace component
#endif // SPRITE_HPP
