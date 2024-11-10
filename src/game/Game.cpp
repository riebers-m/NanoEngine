//
// Created by HP on 10.10.2024.
//

#include "game/Game.hpp"
#include <SDL.h>
#include <glm/glm.hpp>
#include <thread>

#include <chrono>
#include "ECS/ECS.hpp"
#include "assetStore/AssetStore.hpp"
#include "common/TileMapLoader.hpp"
#include "components/Animation.hpp"
#include "components/BoxColider.hpp"
#include "components/CameraFollow.hpp"
#include "components/Health.hpp"
#include "components/KeyboardControlled.hpp"
#include "components/ProjectileEmitter.hpp"
#include "components/RigidBody.hpp"
#include "components/Sprite.hpp"
#include "components/Transform.hpp"
#include "config/Configuration.hpp"
#include "const/Const.hpp"
#include "eventBus/EventBus.hpp"
#include "events/KeyPressedEvent.hpp"
#include "systems/Animation.hpp"
#include "systems/CameraMovement.hpp"
#include "systems/Collision.hpp"
#include "systems/Damage.hpp"
#include "systems/KeyboardMovement.hpp"
#include "systems/Movement.hpp"
#include "systems/ProjectileEmitter.hpp"
#include "systems/ProjectileLifecycle.hpp"
#include "systems/Render.hpp"
#include "systems/RenderCollision.hpp"

using namespace std::chrono_literals;

namespace engine {
    static bool draw_collsion_bb = true;

    Game::Game(SDL_Window *window, SDL_Renderer *renderer, Logger logger, Configuration const &config) :
        m_window(window, [](SDL_Window *w) { SDL_DestroyWindow(w); }),
        m_renderer(renderer, [](SDL_Renderer *r) { SDL_DestroyRenderer(r); }), m_is_running(true), m_timer{},
        m_logger(logger), m_registry{std::make_unique<ecs::Registry>(logger)},
        m_asset_store{std::make_unique<AssetStore>(logger)}, m_event_bus{std::make_unique<events::EventBus>(m_logger)},
        m_config{config}, m_camera{0, 0, config.window_width, config.window_height} {}

    Game::~Game() {}

    void Game::run() {
        setup();

        while (m_is_running) {

            auto const dt = variable_time();
            // auto const dt = fixed_time();
            process_input();
            update(dt);
            render();
        }
    }

    void Game::setup() {
        m_logger->info("ms per frame: {}", engine::MILL_SEC_PER_FRAME.count());
        load_level(1);

        m_timer.start();
    }
    void Game::load_level(std::size_t level) {
        m_registry->add_system<systems::Movement>(m_logger);
        m_registry->add_system<systems::RenderSystem>(m_logger);
        m_registry->add_system<systems::Animation>(m_logger);
        m_registry->add_system<systems::Collision>(m_logger);
        m_registry->add_system<systems::RenderCollision>(m_logger);
        m_registry->add_system<systems::Damage>(m_logger);
        m_registry->add_system<systems::KeyboardMovement>(m_logger);
        m_registry->add_system<systems::CameraMovement>(m_logger);
        m_registry->add_system<systems::ProjectileEmitter>(m_logger, m_registry.get());
        m_registry->add_system<systems::ProjectileLifecycle>(m_logger);

        m_asset_store->add_texture(m_renderer.get(), {"tank-image"}, engine::TANK_RIGHT);
        m_asset_store->add_texture(m_renderer.get(), {"truck-image"}, engine::TRUCK_RIGHT);
        m_asset_store->add_texture(m_renderer.get(), "tilemap", engine::JUNGLE_MAP);
        m_asset_store->add_texture(m_renderer.get(), "chopper-image", engine::CHOPPER_SPRITESHEET);
        m_asset_store->add_texture(m_renderer.get(), "radar-image", engine::RADAR);
        m_asset_store->add_texture(m_renderer.get(), "bullet-image", engine::BULLET);

        TileMapLoader loader{m_logger};
        loader.load_index_map(engine::JUNGLE_INDEX_MAP);

        auto row = 0;
        auto col = 0;
        auto const scale = 2.0;
        for (auto const &index_row: loader) {
            col = 0;
            for (auto const map_index: index_row) {
                auto tile = m_registry->create_entity();
                tile.add_component<component::Transform>(
                        glm::vec2{engine::TILE_SIZE * col * scale, engine::TILE_SIZE * row * scale},
                        glm::vec2{scale, scale});
                auto const src_X = map_index % engine::JUNGLE_MAP_COLS;
                auto const src_Y = map_index / engine::JUNGLE_MAP_COLS;

                if (src_Y >= engine::JUNGLE_MAP_ROWS) {
                    throw std::runtime_error(
                            std::format("invalid y index ({}) for tile with index {}", src_Y, map_index));
                }
                tile.group("tiles");
                tile.add_component<component::Sprite>("tilemap", engine::TILE_SIZE, engine::TILE_SIZE, 0,
                                                      component::Sprite::Position::free, engine::TILE_SIZE * src_X,
                                                      engine::TILE_SIZE * src_Y);
                ++col;
            }
            ++row;
        }
        m_config.map_width = static_cast<size_t>(loader.map_width() * loader.tile_size() * scale);
        m_config.map_height = static_cast<size_t>(loader.map_height() * loader.tile_size() * scale);

        auto chopper = m_registry->create_entity();
        chopper.tag("player");
        chopper.add_component<component::Transform>(glm::vec2{10.0, 100.0}, glm::vec2{1.0, 1.0}, 0.0);
        chopper.add_component<component::RigidBody>(glm::vec2{0.0, 0.0});
        chopper.add_component<component::Sprite>("chopper-image", 32, 32, 1);
        chopper.add_component<component::Animation>(2, 15, component::animate::infinite);
        chopper.add_component<component::BoxColider>(32, 32);
        chopper.add_component<component::KeyboardControlled>(glm::vec2{0, -80}, glm::vec2{80, 0}, glm::vec2{0, 80},
                                                             glm::vec2{-80, 0});
        chopper.add_component<component::CameraFollow>();
        chopper.add_component<component::Health>();
        chopper.add_component<component::ProjectileEmitter>(glm::vec2{120.0, 120.0}, 0s, 10s,
                                                            component::ProjectileEmitter::Attitude::friendly, 10,
                                                            component::ProjectileEmitter::Repeater::manual);

        auto radar = m_registry->create_entity();
        radar.add_component<component::Transform>(glm::vec2{m_config.window_width - 74, 10.0}, glm::vec2{1.0, 1.0},
                                                  0.0);
        radar.add_component<component::RigidBody>(glm::vec2{0.0, 0.0});
        radar.add_component<component::Sprite>("radar-image", 64, 64, 2, component::Sprite::Position::fixed);
        radar.add_component<component::Animation>(8, 10, component::animate::infinite);

        auto tank = m_registry->create_entity();
        tank.group("enemies");
        tank.add_component<component::Transform>(glm::vec2{200.0, 10.0}, glm::vec2{1.0, 1.0}, 0.0);
        tank.add_component<component::RigidBody>(glm::vec2{0.0, 0.0});
        tank.add_component<component::Sprite>("tank-image", 32, 32, 2);
        tank.add_component<component::BoxColider>(32, 32);
        tank.add_component<component::ProjectileEmitter>(glm::vec2{0.0, 100.0}, 5s, 3s,
                                                         component::ProjectileEmitter::Attitude::enemy, 10);
        tank.add_component<component::Health>();
        auto truck = m_registry->create_entity();
        truck.group("enemies");
        truck.add_component<component::Transform>(glm::vec2{600.0, 10.0}, glm::vec2{1.0, 1.0}, 0.0);
        truck.add_component<component::RigidBody>(glm::vec2{0.0, 0.0});
        truck.add_component<component::Sprite>("truck-image", 32, 32, 2);
        truck.add_component<component::BoxColider>(32, 32);
        truck.add_component<component::ProjectileEmitter>(glm::vec2{0.0, 80.0}, 3s, 3s,
                                                          component::ProjectileEmitter::Attitude::enemy, 10);
        truck.add_component<component::Health>();
    }

    float Game::variable_time() {
        if (auto time_to_wait = engine::MILL_SEC_PER_FRAME - m_timer.round();
            time_to_wait > std::chrono::milliseconds(0) && time_to_wait <= engine::MILL_SEC_PER_FRAME) {
            std::this_thread::sleep_for(time_to_wait);
        }

        float const delta_time = static_cast<float>(m_timer.round().count() / 1000.0);
        m_timer.start();
        return delta_time;
    }

    float Game::fixed_time() {
        if (auto time_to_wait = engine::MILL_SEC_PER_FRAME - m_timer.round();
            time_to_wait > std::chrono::milliseconds(0) && time_to_wait <= engine::MILL_SEC_PER_FRAME) {
            std::this_thread::sleep_for(time_to_wait);
        }
        m_timer.start();
        return 1;
    }


    void Game::process_input() {
        SDL_Event evnt = {};
        while (SDL_PollEvent(&evnt)) {
            switch (evnt.type) {
                case SDL_QUIT:
                    m_is_running = false;
                    break;
                case SDL_KEYDOWN:
                    if (evnt.key.keysym.sym == SDLK_ESCAPE) {
                        m_is_running = false;
                    }
                    if (evnt.key.keysym.sym == SDLK_d) {
                        draw_collsion_bb = !draw_collsion_bb;
                    }
                    m_event_bus->emit<events::KeyPressedEvent>(evnt.key.keysym.sym);
                default:
                    break;
            }
        }
    }
    void Game::update(float dt) {
        // reset all event handlers for the current frame
        m_event_bus->clear_subscribers();

        // systems subscribe to events
        m_registry->get_system<systems::Damage>().subscribe_to_event(m_event_bus.get());
        m_registry->get_system<systems::ProjectileEmitter>().subscribe_to_event(m_event_bus.get());
        m_registry->get_system<systems::KeyboardMovement>().subscribe_to_event(m_event_bus.get());

        m_registry->update();
        m_registry->get_system<systems::Movement>().update(dt);
        m_registry->get_system<systems::ProjectileEmitter>().update();
        m_registry->get_system<systems::Collision>().update(m_event_bus.get());
        m_registry->get_system<systems::Animation>().update(dt);
        m_registry->get_system<systems::CameraMovement>().update(m_camera, m_config);
        m_registry->get_system<systems::ProjectileLifecycle>().update();
    }
    void Game::render() const {
        SDL_SetRenderDrawColor(m_renderer.get(), 21, 21, 21, 255);
        SDL_RenderClear(m_renderer.get());
        try {
            m_registry->get_system<systems::RenderSystem>().update(m_renderer.get(), m_asset_store.get(), m_camera);

            if (draw_collsion_bb) {
                m_registry->get_system<systems::RenderCollision>().update(m_renderer.get(), m_camera);
            }
        } catch (std::exception const &e) {
            m_logger->error("render system error: {}", e.what());
        }
        SDL_RenderPresent(m_renderer.get());
    }
} // namespace engine
