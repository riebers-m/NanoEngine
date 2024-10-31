//
// Created by HP on 10.10.2024.
//

#include "game/Engine.hpp"
#include <SDL.h>
#include <format>
#include "spdlog/sinks/stdout_color_sinks.h"

namespace engine {
    Engine::Engine() {
        if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
            throw std::runtime_error(std::format("init SDL2 failed: {}", SDL_GetError()));
        }
    }

    Engine::~Engine() { SDL_Quit(); }

    Engine const &Engine::instance() {
        static Engine eng = Engine{};
        return eng;
    }

    Game Engine::create(Engine const &) {
        uint16_t window_width{}, window_height{};
        SDL_DisplayMode display_mode;
        if (SDL_GetCurrentDisplayMode(0, &display_mode) < 0) {
            throw std::runtime_error(std::format("could not get display mode: {}", SDL_GetError()));
        }
        // window_width = static_cast<uint16_t>(display_mode.w);
        // window_height = static_cast<uint16_t>(display_mode.h);

        window_width = 1440;
        window_height = 900;
        SDL_Window *window = SDL_CreateWindow("2D Game Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                              window_width, window_height, SDL_WINDOW_BORDERLESS);
        if (window == nullptr) {
            throw std::runtime_error("could not create window.");
        }
        SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        if (renderer == nullptr) {
            throw std::runtime_error("could not create renderer.");
        }

        if (SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN) < 0) {
            throw std::runtime_error(std::format("could not set window to fullscreen video mode: {}", SDL_GetError()));
        }

        auto logger = spdlog::stdout_color_mt("console", spdlog::color_mode::automatic);
        logger->set_level(spdlog::level::debug);
        return Game{window, renderer, logger, window_width, window_height};
    }
} // namespace engine
