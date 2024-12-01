//
// Created by HP on 11.10.2024.
//

#ifndef CONST_HPP
#define CONST_HPP
#include <chrono>
#include <filesystem>
#include <string>

namespace engine {
#ifdef _DEBUG
    std::filesystem::path const ASSET_PATH(R"(C:\Users\HP\dev\custom_game_engine\assets)");
#elif
    std::string const ASSET_PATH = "./assets";
#endif

    std::filesystem::path const FONTS_PATH = ASSET_PATH / "fonts";
    std::filesystem::path const IMAGES_PATH = ASSET_PATH / "images";
    std::filesystem::path const TILEMAPS_PATH = ASSET_PATH / "tilemaps";
    std::filesystem::path const TANK_RIGHT = IMAGES_PATH / "tank-tiger-right.png";
    std::filesystem::path const TRUCK_RIGHT = IMAGES_PATH / "truck-ford-right.png";
    std::filesystem::path const JUNGLE_MAP = TILEMAPS_PATH / "jungle.png";
    std::filesystem::path const CHOPPER = IMAGES_PATH / "chopper.png";
    std::filesystem::path const CHOPPER_SPRITESHEET = IMAGES_PATH / "chopper-spritesheet.png";
    std::filesystem::path const RADAR = IMAGES_PATH / "radar.png";
    std::filesystem::path const BULLET = IMAGES_PATH / "bullet.png";
    std::filesystem::path const TREE = IMAGES_PATH / "tree.png";

    std::filesystem::path const JUNGLE_INDEX_MAP = TILEMAPS_PATH / "jungle.map";

    std::filesystem::path const CHARRIOT_FONT = FONTS_PATH / "charriot.ttf";


    std::chrono::milliseconds constexpr FPS = std::chrono::milliseconds(60);
    std::chrono::milliseconds constexpr MILL_SEC_PER_FRAME =
            std::chrono::milliseconds(std::chrono::milliseconds(1000) / FPS);

    std::size_t constexpr MAX_COMPONENTS = 32;
    int constexpr TILE_SIZE = 32;
    int constexpr JUNGLE_MAP_ROWS = 3;
    int constexpr JUNGLE_MAP_COLS = 10;

} // namespace engine

#endif // CONST_HPP
