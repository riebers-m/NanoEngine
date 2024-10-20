//
// Created by HP on 10.10.2024.
//

#ifndef GAME_HPP
#define GAME_HPP
#include <memory>
#include <spdlog/spdlog.h>

#include "ECS/ECS.hpp"
#include "timer/Timer.hpp"

class SDL_Window;
class SDL_Renderer;

namespace engine {

    class Game {
        using logger = std::shared_ptr<spdlog::logger>;

    private:
        std::unique_ptr<SDL_Window, void (*)(SDL_Window *)> m_window;
        std::unique_ptr<SDL_Renderer, void (*)(SDL_Renderer *)> m_renderer;
        bool m_is_running;
        Timer m_timer;
        std::shared_ptr<spdlog::logger> m_logger;
        ecs::Registry m_registry;

        void process_input();
        void update(float dt);
        void render() const;
        void setup();

        float variable_time(); // pixels per time
        float fixed_time(); // pixels per frame

    public:
        explicit Game(SDL_Window *window, SDL_Renderer *renderer, logger logger);

        void run();
    };

} // namespace engine

#endif // GAME_HPP
