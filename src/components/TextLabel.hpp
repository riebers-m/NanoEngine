//
// Created by HP on 21.11.2024.
//

#ifndef TEXTLABEL_HPP
#define TEXTLABEL_HPP
#include <SDL_pixels.h>
#include <string>

#include "Common.hpp"
#include "glm/vec2.hpp"

namespace component {
    struct TextLabel {
        glm::vec2 position;
        std::string text;
        std::string asset_id;
        SDL_Color color;
        WorldPosition world_position;


        explicit TextLabel(glm::vec2 const &position = {0, 0}, std::string const &text = "",
                           std::string const &asset_id = "", SDL_Color const &color = {},
                           WorldPosition world_position = WorldPosition::fixed) :
            position{position}, text{text}, asset_id{asset_id}, color{color}, world_position{world_position} {}
    };
} // namespace component

#endif // TEXTLABEL_HPP
