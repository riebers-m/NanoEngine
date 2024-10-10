//
// Created by HP on 10.10.2024.
//

#include "Engine.hpp"
#include <format>
#include <iostream>
#include "SDL.h"
namespace engine {
    Engine::Engine() {
        if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
            throw std::runtime_error(std::format("init SDL2 failed: {}", SDL_GetError()));
        }
    }

    Engine::~Engine() {
        std::cout << "destroying SDL\n";
        SDL_Quit();
    }

    Engine const &Engine::instance() {
        static Engine eng = Engine{};
        return eng;
    }

    Game Engine::create(Engine const &) {
        int window_width{}, window_height{};
        SDL_DisplayMode display_mode;
        if (SDL_GetCurrentDisplayMode(0, &display_mode) < 0) {
            throw std::runtime_error(std::format("could not get display mode: {}", SDL_GetError()));
        }
        window_width = display_mode.w;
        window_height = display_mode.h;

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

        return Game{window, renderer};
    }
} // namespace engine
