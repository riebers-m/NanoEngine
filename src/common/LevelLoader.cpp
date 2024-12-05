//
// Created by HP on 03.12.2024.
//
#include "LevelLoader.hpp"
#include "ECS/ECS.hpp"
#include "assetStore/AssetStore.hpp"
#include "common/TileMapLoader.hpp"
#include "components/Animation.hpp"
#include "components/BoxCollider.hpp"
#include "components/CameraFollow.hpp"
#include "components/Health.hpp"
#include "components/KeyboardControlled.hpp"
#include "components/ProjectileEmitter.hpp"
#include "components/RigidBody.hpp"
#include "components/Sprite.hpp"
#include "components/TextLabel.hpp"
#include "components/Transform.hpp"
#include "const/Const.hpp"
#include "glm/vec2.hpp"
#include "sol/sol.hpp"

using namespace std::chrono_literals;

namespace engine {
    Configuration LevelLoader::load_level(sol::state &lua, ecs::Registry *registry, engine::AssetStore *asset_store,
                                          SDL_Renderer *renderer, int level) {
        auto const script_path = engine::ASSET_PATH / "scripts" / std::format("level{}.lua", level);
        sol::load_result const script = lua.load_file(script_path.string());

        if (!script.valid()) {
            sol::error err = script;
            throw std::runtime_error(
                    std::format("could not load script: {} - message: {}", script_path.string(), err.what()));
        }
        Configuration config{};
        lua.script_file(script_path.string());
        // Load entities and compontens from lua script

        //
        // asset_store->add_texture(renderer, {"tank-image"}, engine::TANK_RIGHT);
        // asset_store->add_texture(renderer, {"truck-image"}, engine::TRUCK_RIGHT);
        // asset_store->add_texture(renderer, "tilemap", engine::JUNGLE_MAP);
        // asset_store->add_texture(renderer, "chopper-image", engine::CHOPPER_SPRITESHEET);
        // asset_store->add_texture(renderer, "radar-image", engine::RADAR);
        // asset_store->add_texture(renderer, "bullet-image", engine::BULLET);
        // asset_store->add_texture(renderer, "tree-image", engine::TREE);
        // asset_store->add_font("charriot-h1", engine::CHARRIOT_FONT, 20);
        // asset_store->add_font("charriot-text", engine::CHARRIOT_FONT, 14);
        //
        // TileMapLoader loader{};
        // loader.load_index_map(engine::JUNGLE_INDEX_MAP);
        //
        // auto row = 0;
        // auto col = 0;
        // auto const scale = 2.0;
        // for (auto const &index_row: loader) {
        //     col = 0;
        //     for (auto const map_index: index_row) {
        //         auto tile = registry->create_entity();
        //         tile.add_component<component::Transform>(
        //                 glm::vec2{engine::TILE_SIZE * col * scale, engine::TILE_SIZE * row * scale},
        //                 glm::vec2{scale, scale});
        //         auto const src_X = map_index % engine::JUNGLE_MAP_COLS;
        //         auto const src_Y = map_index / engine::JUNGLE_MAP_COLS;
        //
        //         if (src_Y >= engine::JUNGLE_MAP_ROWS) {
        //             throw std::runtime_error(
        //                     std::format("invalid y index ({}) for tile with index {}", src_Y, map_index));
        //         }
        //         tile.group("tiles");
        //         tile.add_component<component::Sprite>("tilemap", engine::TILE_SIZE, engine::TILE_SIZE, 0,
        //                                               component::WorldPosition::free, engine::TILE_SIZE * src_X,
        //                                               engine::TILE_SIZE * src_Y);
        //         ++col;
        //     }
        //     ++row;
        // }
        //
        // Configuration config{};
        // config.map_width = static_cast<size_t>(loader.map_width() * loader.tile_size() * scale);
        // config.map_height = static_cast<size_t>(loader.map_height() * loader.tile_size() * scale);
        //
        // auto chopper = registry->create_entity();
        // chopper.tag("player");
        // chopper.add_component<component::Transform>(glm::vec2{240.0, 110.0}, glm::vec2{1.0, 1.0}, 0.0);
        // chopper.add_component<component::RigidBody>(glm::vec2{0.0, 0.0});
        // chopper.add_component<component::Sprite>("chopper-image", 32, 32, 1);
        // chopper.add_component<component::Animation>(2, 15, component::animate::infinite);
        // chopper.add_component<component::BoxCollider>(32, 32);
        // chopper.add_component<component::KeyboardControlled>(glm::vec2{0, -80}, glm::vec2{80, 0}, glm::vec2{0, 80},
        //                                                      glm::vec2{-80, 0});
        // chopper.add_component<component::CameraFollow>();
        // chopper.add_component<component::Health>();
        // chopper.add_component<component::ProjectileEmitter>(glm::vec2{120.0, 120.0}, 0s, 10s,
        //                                                     component::ProjectileEmitter::Attitude::friendly, 10,
        //                                                     component::ProjectileEmitter::Repeater::manual);
        //
        // auto radar = registry->create_entity();
        // radar.add_component<component::Transform>(glm::vec2{config.window_width - 74, 10.0}, glm::vec2{1.0, 1.0},
        // 0.0); radar.add_component<component::RigidBody>(glm::vec2{0.0, 0.0});
        // radar.add_component<component::Sprite>("radar-image", 64, 64, 2, component::WorldPosition::fixed);
        // radar.add_component<component::Animation>(8, 10, component::animate::infinite);
        //
        // auto tank = registry->create_entity();
        // tank.group("enemies");
        // tank.add_component<component::Transform>(glm::vec2{500.0, 500.0}, glm::vec2{1.0, 1.0}, 0.0);
        // tank.add_component<component::RigidBody>(glm::vec2{20.0, 0.0});
        // tank.add_component<component::Sprite>("tank-image", 32, 32, 2);
        // tank.add_component<component::BoxCollider>(32, 32);
        // // tank.add_component<component::ProjectileEmitter>(glm::vec2{0.0, 100.0}, 5s, 3s,
        // //                                                  component::ProjectileEmitter::Attitude::enemy, 10);
        // tank.add_component<component::Health>();
        // auto truck = registry->create_entity();
        // truck.group("enemies");
        // truck.add_component<component::Transform>(glm::vec2{120.0, 500.0}, glm::vec2{1.0, 1.0}, 0.0);
        // truck.add_component<component::RigidBody>(glm::vec2{0.0, 0.0});
        // truck.add_component<component::Sprite>("truck-image", 32, 32, 2);
        // truck.add_component<component::BoxCollider>(32, 32);
        // truck.add_component<component::ProjectileEmitter>(glm::vec2{0.0, 80.0}, 3s, 3s,
        //                                                   component::ProjectileEmitter::Attitude::enemy, 10);
        // truck.add_component<component::Health>();
        //
        //
        // {
        //     ecs::Entity tree = registry->create_entity();
        //     tree.group("obstacles");
        //     tree.add_component<component::Transform>(glm::vec2{600.0, 495.0}, glm::vec2{1.0, 1.0}, 0.0);
        //     tree.add_component<component::Sprite>("tree-image", 16, 32, 2);
        //     tree.add_component<component::BoxCollider>(16, 32);
        // }
        //
        //
        // {
        //     ecs::Entity tree = registry->create_entity();
        //     tree.group("obstacles");
        //     tree.add_component<component::Transform>(glm::vec2{400.0, 495.0}, glm::vec2{1.0, 1.0}, 0.0);
        //     tree.add_component<component::Sprite>("tree-image", 16, 32, 2);
        //     tree.add_component<component::BoxCollider>(16, 32);
        // }
        //
        // auto label = registry->create_entity();
        // SDL_Color white = {255, 255, 255};
        // SDL_Color green = {0, 255, 0};
        // label.add_component<component::TextLabel>(glm::vec2{config.window_width / 2 - 40, 10}, "CHOPPER v1.0",
        //                                           "charriot-h1", green);
        return config;
    }
} // namespace engine
