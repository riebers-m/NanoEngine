//
// Created by HP on 10.10.2024.
//

#include "Game.hpp"
#include <SDL.h>
#include <iostream>

namespace engine {
    Game::Game(SDL_Window *window, SDL_Renderer *renderer) :
        m_window(window,
                 [](SDL_Window *w) {
                     std::cout << "destroying window\n";
                     SDL_DestroyWindow(w);
                 }),
        m_renderer(renderer,
                   [](SDL_Renderer *r) {
                       std::cout << "destroying renderer\n";
                       SDL_DestroyRenderer(r);
                   }),
        m_is_running(true) {}
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
    void Game::update() {}
    void Game::render() {
        SDL_SetRenderDrawColor(m_renderer.get(), 255, 0, 0, 255);
        SDL_RenderClear(m_renderer.get());

        SDL_RenderPresent(m_renderer.get());
    }
    void Game::setup() {}

    void Game::run() {
        while (m_is_running) {
            process_input();
            update();
            render();
        }
    }
} // namespace engine
