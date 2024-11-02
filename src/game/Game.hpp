//
// Created by HP on 10.10.2024.
//

#ifndef GAME_HPP
#define GAME_HPP
#include <memory>
#include <spdlog/spdlog.h>
#include "timer/Timer.hpp"

namespace ecs {
    class Registry;
}
struct SDL_Window;
struct SDL_Renderer;
namespace events {
    class EventBus;
}

namespace engine {
    class AssetStore;

    class Game {
        using Logger = std::shared_ptr<spdlog::logger>;

    private:
        std::unique_ptr<SDL_Window, void (*)(SDL_Window *)> m_window;
        std::unique_ptr<SDL_Renderer, void (*)(SDL_Renderer *)> m_renderer;
        bool m_is_running;
        Timer m_timer;
        std::shared_ptr<spdlog::logger> m_logger;
        std::unique_ptr<ecs::Registry> m_registry;
        std::unique_ptr<AssetStore> m_asset_store;
        std::unique_ptr<events::EventBus> m_event_bus;
        uint16_t m_window_width;
        uint16_t m_window_height;

        void process_input();
        void update(float dt);
        void render() const;
        void setup();
        void load_level(std::size_t level);

        float variable_time(); // pixels per time
        float fixed_time(); // pixels per frame

    public:
        explicit Game(SDL_Window *window, SDL_Renderer *renderer, Logger logger, uint16_t window_width,
                      uint16_t window_height);
        ~Game(); // needs to be defined otherwise compiler tries to generate it and does not know at this point how to
                 // destroy forward declared registry
        void run();
    };

} // namespace engine

#endif // GAME_HPP
