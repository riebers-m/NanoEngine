//
// Created by HP on 10.10.2024.
//

#include "game/Game.hpp"
#include <SDL.h>
#include <glm/glm.hpp>
#include <thread>

#include "ECS/ECS.hpp"
#include "assetStore/AssetStore.hpp"
#include "common/TileMapLoader.hpp"
#include "components/RigidBody.hpp"
#include "components/Sprite.hpp"
#include "components/Transform.hpp"
#include "const/Const.hpp"
#include "systems/Movement.hpp"
#include "systems/Render.hpp"

namespace engine {
    Game::Game(SDL_Window *window, SDL_Renderer *renderer, logger logger) :
        m_window(window, [](SDL_Window *w) { SDL_DestroyWindow(w); }),
        m_renderer(renderer, [](SDL_Renderer *r) { SDL_DestroyRenderer(r); }), m_is_running(true), m_timer{},
        m_logger(logger) {

        m_registry = std::make_unique<ecs::Registry>(logger);
        m_asset_store = std::make_unique<AssetStore>(logger);
    }

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

        m_asset_store->add_texture(m_renderer.get(), {"tank-image"}, engine::TANK_RIGHT);
        m_asset_store->add_texture(m_renderer.get(), {"truck-image"}, engine::TRUCK_RIGHT);
        m_asset_store->add_texture(m_renderer.get(), "tilemap", engine::JUNGLE_MAP);

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
                tile.add_component<component::Sprite>("tilemap", engine::TILE_SIZE, engine::TILE_SIZE, 0,
                                                      engine::TILE_SIZE * src_X, engine::TILE_SIZE * src_Y);
                ++col;
            }
            ++row;
        }
        auto tank = m_registry->create_entity();
        tank.add_component<component::Transform>(glm::vec2{10.0, 10.0}, glm::vec2{1.0, 1.0}, 0.0);
        tank.add_component<component::RigidBody>(glm::vec2{30.0, 0.0});
        tank.add_component<component::Sprite>("tank-image", 32, 32, 2);

        auto truck = m_registry->create_entity();
        truck.add_component<component::Transform>(glm::vec2{10.0, 10.0}, glm::vec2{1.0, 1.0}, 0.0);
        truck.add_component<component::RigidBody>(glm::vec2{20.0, 0.0});
        truck.add_component<component::Sprite>("truck-image", 32, 32, 1);
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
                default:
                    break;
            }
        }
    }
    void Game::update(float dt) {
        m_registry->update();
        m_registry->get_system<systems::Movement>().update(dt);
    }
    void Game::render() const {
        SDL_SetRenderDrawColor(m_renderer.get(), 21, 21, 21, 255);
        SDL_RenderClear(m_renderer.get());
        try {
            m_registry->get_system<systems::RenderSystem>().update(m_renderer.get(), m_asset_store.get());
        } catch (std::exception const &e) {
            m_logger->error("render system error: {}", e.what());
        }
        SDL_RenderPresent(m_renderer.get());
    }
} // namespace engine
