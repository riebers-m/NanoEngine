//
// Created by HP on 03.12.2024.
//
#include "LevelLoader.hpp"

#include <utility>
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
    LevelLoader::LevelLoader(Logger logger) : m_logger{std::move(logger)} {}
    Configuration LevelLoader::load_level(sol::state &lua, ecs::Registry *registry, engine::AssetStore *asset_store,
                                          SDL_Renderer *renderer, int level_id) {
        auto const script_path = engine::ASSET_PATH / "scripts" / std::format("level{}.lua", level_id);
        sol::load_result const script = lua.load_file(script_path.string());

        if (!script.valid()) {
            sol::error err = script;
            throw std::runtime_error(
                    std::format("could not load script: {} - message: {}", script_path.string(), err.what()));
        }
        // Load entities and compontens from lua script
        lua.script_file(script_path.string());

        sol::table const level = lua["Level"];
        /////
        /// Read the level assets
        /////
        sol::table const assets = level["assets"];
        {
            int i = 0;
            while (true) {
                sol::optional<sol::table> const has_asset = assets[i];
                if (has_asset == sol::nullopt) {
                    break;
                }
                sol::table const asset = assets[i];
                std::string const asset_type = asset["type"];
                std::string const asset_id = asset["id"];
                if (asset_type == "texture") {
                    asset_store->add_texture(renderer, asset_id, engine::ASSET_PATH / asset["file"].get<std::string>());
                    m_logger->debug("New texture asset loaded to the asset store, id: {}",
                                    asset["id"].get<std::string>());
                }
                if (asset_type == "font") {
                    asset_store->add_font(asset_id, engine::ASSET_PATH / asset["file"].get<std::string>(),
                                          asset["font_size"].get<std::size_t>());
                    m_logger->debug("New font asset loaded to the asset store, id: {}", asset["id"].get<std::string>());
                }
                ++i;
            }
        }

        /////
        /// Read the tilemap
        /////
        ///
        Configuration config{};
        {

            sol::table const tilemap = level["tilemap"];
            std::filesystem::path const map_file = engine::ASSET_PATH / tilemap["map_file"].get<std::string>();

            std::size_t const num_rows = tilemap["num_rows"].get<std::size_t>();
            std::size_t const num_cols = tilemap["num_cols"].get<std::size_t>();
            int const tile_size = tilemap["tile_size"].get<int>();
            float const scale = tilemap["scale"].get<float>();
            std::string const texture_asset_id = tilemap["texture_asset_id"].get<std::string>();

            TileMapLoader loader{tile_size};
            loader.load_index_map(map_file);
            if (loader.tile_size() != tile_size) {
                throw std::runtime_error(std::format("lua definition and map file diverged: tile_size {} != {}",
                                                     loader.tile_size(), tile_size));
            }
            if (loader.map_width() != num_cols * tile_size) {
                throw std::runtime_error(std::format("lua definition and map file diverged: map width {} != {}",
                                                     loader.map_width(), num_cols * tile_size));
            }
            if (loader.map_height() != num_rows * tile_size) {
                throw std::runtime_error(std::format("lua definition and map file diverged: map height {} != {}",
                                                     loader.map_height(), num_rows * tile_size));
            }


            auto col = 0;
            auto row = 0;
            for (auto const &index_row: loader) {
                col = 0;
                for (auto const [x_coord, y_coord]: index_row) {
                    auto tile = registry->create_entity();
                    tile.add_component<component::Transform>(
                            glm::vec2{static_cast<float>(tile_size) * static_cast<float>(col) * scale,
                                      static_cast<float>(tile_size) * static_cast<float>(row) * scale},
                            glm::vec2{scale, scale});
                    tile.add_component<component::Sprite>(texture_asset_id, tile_size, tile_size, 0,
                                                          component::WorldPosition::free, tile_size * x_coord,
                                                          tile_size * y_coord);
                    tile.group("tiles");
                    ++col;
                }
                ++row;
            }
            config.map_width = loader.map_width() * scale;
            config.map_height = loader.map_height() * scale;
        }


        /////
        /// Read the entities
        /////
        {
            sol::table const entities = level["entities"];
            int i = 0;
            while (true) {
                sol::optional<sol::table> const has_entity = entities[i];
                if (has_entity == sol::nullopt) {
                    break;
                }
                sol::table const entity = entities[i];

                sol::table const components = entity["components"];

                ecs::Entity new_entity = registry->create_entity();

                if (sol::optional<sol::table> const has_transform = components["transform"];
                    has_transform != sol::nullopt) {
                    sol::table const transform = components["transform"];
                    sol::table const position = transform["position"];
                    sol::table const scale = transform["scale"];

                    new_entity.add_component<component::Transform>(
                            glm::vec2{position["x"].get<float>(), position["y"].get<float>()},
                            glm::vec2{scale["x"].get_or<float>(1.0), scale["y"].get_or<float>(1.0)},
                            transform["rotation"].get_or<float>(0.0));

                    m_logger->debug("Added transform component");
                }

                if (sol::optional<sol::table> const has_rigidbody = components["rigidbody"];
                    has_rigidbody != sol::nullopt) {
                    sol::table const rigidbody = components["rigidbody"];
                    sol::table const velocity = rigidbody["velocity"];
                    new_entity.add_component<component::RigidBody>(
                            glm::vec2{velocity["x"].get_or<float>(0.0), velocity["y"].get_or<float>(0.0)});
                    m_logger->debug("Added rigidbody component");
                }
                if (sol::optional<sol::table> const has_sprite = components["sprite"]; has_sprite != sol::nullopt) {
                    sol::table const sprite = components["sprite"];
                    auto const texture_id = sprite["texture_asset_id"].get<std::string>();
                    auto const world_position = sprite["world_position"].get_or<std::string>("free");

                    new_entity.add_component<component::Sprite>(
                            texture_id, sprite["width"].get<int>(), sprite["height"].get<int>(),
                            sprite["z_index"].get_or<int>(1.0), component::from_string(world_position),
                            sprite["src_rect_x"].get_or<int>(0.0), sprite["src_rect_y"].get_or<int>(0.0));


                    m_logger->debug("Added sprite component");
                }

                if (sol::optional<sol::table> const has_animation = components["animation"];
                    has_animation != sol::nullopt) {
                    sol::table const animation = components["animation"];

                    new_entity.add_component<component::Animation>(animation["num_frames"].get_or<int>(1.0),
                                                                   animation["speed_rate"].get_or<int>(1.0));

                    m_logger->debug("Added animation component");
                }

                if (sol::optional<sol::table> const has_boxcollider = components["boxcollider"];
                    has_boxcollider != sol::nullopt) {
                    sol::table const boxcollider = components["boxcollider"];

                    new_entity.add_component<component::BoxCollider>(
                            boxcollider["width"].get<int>(), boxcollider["height"].get<int>(),
                            glm::vec2{boxcollider["offset"]["x"].get_or<float>(0.0),
                                      boxcollider["offset"]["y"].get_or<float>(0.0)});

                    m_logger->debug("Added boxcollider component");
                }

                if (sol::optional<sol::table> const has_health = components["health"]; has_health != sol::nullopt) {
                    sol::table const health = components["health"];

                    new_entity.add_component<component::Health>(health["health_percentage"].get_or<int>(100.0));

                    m_logger->debug("Added health component");
                }

                if (sol::optional<sol::table> const has_projectile_emitter = components["projectile_emitter"];
                    has_projectile_emitter != sol::nullopt) {
                    sol::table const projectile_emitter = components["projectile_emitter"];

                    new_entity.add_component<component::ProjectileEmitter>(
                            glm::vec2{projectile_emitter["projectile_velocity"]["x"].get<float>(),
                                      projectile_emitter["projectile_velocity"]["y"].get<float>()},
                            std::chrono::milliseconds{projectile_emitter["repeat_frequency"].get<int>() * 1000},
                            std::chrono::milliseconds{projectile_emitter["projectile_duration"].get<int>() * 1000},
                            component::ProjectileEmitter::string_to_attitude(
                                    projectile_emitter["attitude"].get_or<std::string>("enemy")),
                            projectile_emitter["hit_percentage_damage"].get_or<int>(10.0),
                            component::ProjectileEmitter::string_to_repeater(
                                    projectile_emitter["repeater"].get_or<std::string>("automatic")));

                    m_logger->debug("Added projectile_emitter component");
                }
                if (sol::optional<sol::table> const has_keyboard_controller = components["keyboard_controller"];
                    has_keyboard_controller != sol::nullopt) {
                    sol::table const keyboard_controller = components["keyboard_controller"];

                    new_entity.add_component<component::KeyboardControlled>(
                            glm::vec2{keyboard_controller["up_velocity"]["x"].get<float>(),
                                      keyboard_controller["up_velocity"]["y"].get<float>()},
                            glm::vec2{keyboard_controller["right_velocity"]["x"].get<float>(),
                                      keyboard_controller["right_velocity"]["y"].get<float>()},
                            glm::vec2{keyboard_controller["down_velocity"]["x"].get<float>(),
                                      keyboard_controller["down_velocity"]["y"].get<float>()},
                            glm::vec2{keyboard_controller["left_velocity"]["x"].get<float>(),
                                      keyboard_controller["left_velocity"]["y"].get<float>()}

                    );

                    m_logger->debug("Added keyboard_controller component");
                }

                if (sol::optional<sol::table> const has_camera_follow = components["camera_follow"];
                    has_camera_follow != sol::nullopt) {
                    sol::table const camera_follow = components["camera_follow"];

                    new_entity.add_component<component::CameraFollow>();

                    m_logger->debug("Added camera_follow component");
                }

                if (sol::optional<std::string> const has_tag = entity["tag"]; has_tag != sol::nullopt) {
                    new_entity.tag(entity["tag"].get<std::string>());
                    m_logger->debug("Added entity with tag {}", entity["tag"].get<std::string>());
                }
                if (sol::optional<std::string> const has_group = entity["group"]; has_group != sol::nullopt) {
                    new_entity.group(entity["group"].get<std::string>());
                    m_logger->debug("Added entity with group {}", entity["group"].get<std::string>());
                }

                ++i;
            }
        }


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
        //                                               component::WorldPosition::free, engine::TILE_SIZE *
        //                                               src_X, engine::TILE_SIZE * src_Y);
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
        // chopper.add_component<component::KeyboardControlled>(glm::vec2{0, -80}, glm::vec2{80, 0},
        // glm::vec2{0, 80},
        //                                                      glm::vec2{-80, 0});
        // chopper.add_component<component::CameraFollow>();
        // chopper.add_component<component::Health>();
        // chopper.add_component<component::ProjectileEmitter>(glm::vec2{120.0, 120.0}, 0s, 10s,
        //                                                     component::ProjectileEmitter::Attitude::friendly,
        //                                                     10,
        //                                                     component::ProjectileEmitter::Repeater::manual);
        //
        // auto radar = registry->create_entity();
        // radar.add_component<component::Transform>(glm::vec2{config.window_width - 74, 10.0},
        // glm::vec2{1.0, 1.0}, 0.0); radar.add_component<component::RigidBody>(glm::vec2{0.0, 0.0});
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
        // //                                                  component::ProjectileEmitter::Attitude::enemy,
        // 10); tank.add_component<component::Health>(); auto truck = registry->create_entity();
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
        // label.add_component<component::TextLabel>(glm::vec2{config.window_width / 2 - 40, 10}, "CHOPPER
        // v1.0",
        //                                           "charriot-h1", green);
        return config;
    }
} // namespace engine
