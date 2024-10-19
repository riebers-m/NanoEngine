//
// Created by HP on 10.10.2024.
//

#include "game/Game.hpp"
#include <SDL.h>
#include <SDL_image.h>
#include <format>
#include <glm/glm.hpp>
#include <thread>

#include "components/RigidBody.hpp"
#include "components/Transform.hpp"
#include "const/Const.hpp"

namespace engine {
    Game::Game(SDL_Window *window, SDL_Renderer *renderer, logger logger) :
        m_window(window, [](SDL_Window *w) { SDL_DestroyWindow(w); }),
        m_renderer(renderer, [](SDL_Renderer *r) { SDL_DestroyRenderer(r); }), m_is_running(true), m_timer{},
        m_logger(logger), m_registry{logger} {}

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
            }
        }
    }
    void Game::update(float dt) const {}
    void Game::render() const {
        SDL_SetRenderDrawColor(m_renderer.get(), 21, 21, 21, 255);
        SDL_RenderClear(m_renderer.get());

        // if (SDL_Surface *surface = IMG_Load(engine::TANK_RIGHT.c_str()); surface != nullptr) {
        //     if (SDL_Texture *texture = SDL_CreateTextureFromSurface(m_renderer.get(), surface); texture != nullptr) {
        //         SDL_FreeSurface(surface);
        //         SDL_Rect dest_rect = {static_cast<int>(player_pos.x), static_cast<int>(player_pos.y), 32, 32};
        //         if (SDL_RenderCopy(m_renderer.get(), texture, nullptr, &dest_rect) < 0) {
        //             std::cerr << std::format("could not render texture {}", SDL_GetError());
        //         }
        //         SDL_DestroyTexture(texture);
        //     } else {
        //         std::cerr << std::format("could not load texture: {}\n", SDL_GetError());
        //     }
        // } else {
        //     std::cerr << std::format("could not load img: {}\n", SDL_GetError());
        // }

        SDL_RenderPresent(m_renderer.get());
    }
    void Game::setup() {
        m_logger->info("ms per frame: {}", engine::MILL_SEC_PER_FRAME.count());

        try {
            auto tank = m_registry.create_entity();
            tank.add_component<component::Transform>(glm::vec2{10.0, 30.0}, glm::vec2{1.0, 1.0}, 1.0);
            tank.add_component<component::RigidBody>(glm::vec2{10.0, 50.0});
            tank.remove_component<component::RigidBody>();

        } catch (std::out_of_range const &e) {
            throw std::runtime_error(e.what());
        }

        m_timer.start();
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
} // namespace engine
