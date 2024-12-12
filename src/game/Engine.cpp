//
// Created by HP on 10.10.2024.
//

#include "game/Engine.hpp"
#include <SDL.h>
#include <SDL_ttf.h>
#include <format>
#include <utility>

namespace engine {
    Engine::Engine() {
        if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
            throw std::runtime_error(std::format("init SDL2 failed: {}", SDL_GetError()));
        }
        if (TTF_Init() < 0) {
            throw std::runtime_error(std::format("init ttf failed: {}", TTF_GetError()));
        }
    }

    Engine::~Engine() {
        SDL_Quit();
        TTF_Quit();
    }

    Engine const &Engine::instance() {
        static Engine eng = Engine{};
        return eng;
    }

    Game Engine::create(std::shared_ptr<Logger> logger, Engine const &) {
        SDL_DisplayMode display_mode;
        if (SDL_GetCurrentDisplayMode(0, &display_mode) < 0) {
            throw std::runtime_error(std::format("could not get display mode: {}", SDL_GetError()));
        }
        // window_width = static_cast<uint16_t>(display_mode.w);
        // window_height = static_cast<uint16_t>(display_mode.h);

        auto constexpr config = Configuration{1440, 900, 0, 0};

        SDL_Window *window = SDL_CreateWindow("2D Game Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                              config.window_width, config.window_height, SDL_WINDOW_BORDERLESS);
        if (window == nullptr) {
            throw std::runtime_error("could not create window.");
        }
        SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        if (renderer == nullptr) {
            throw std::runtime_error("could not create renderer.");
        }

        // if (SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN) < 0) {
        //     throw std::runtime_error(std::format("could not set window to fullscreen video mode: {}",
        //     SDL_GetError()));
        // }

        return Game{window, renderer, std::move(logger), config};
    }
} // namespace engine
