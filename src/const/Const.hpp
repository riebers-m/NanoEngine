//
// Created by HP on 11.10.2024.
//

#ifndef CONST_HPP
#define CONST_HPP
#include <chrono>
#include <string>

namespace engine {
#ifdef _DEBUG
    std::string const ASSET_PATH = "C:/Users/HP/dev/custom_game_engine/assets";
#elif
    std::string const ASSET_PATH = "./assets";
#endif

    std::string const IMAGES_PATH = ASSET_PATH + "/images";
    std::string const TANK_RIGHT = IMAGES_PATH + "/tank-tiger-right.png";

    std::chrono::milliseconds constexpr FPS = std::chrono::milliseconds(60);
    std::chrono::milliseconds constexpr MILL_SEC_PER_FRAME =
            std::chrono::milliseconds(std::chrono::milliseconds(1000) / FPS);

} // namespace engine

#endif // CONST_HPP
