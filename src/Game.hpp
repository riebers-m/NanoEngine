//
// Created by HP on 10.10.2024.
//

#ifndef GAME_HPP
#define GAME_HPP
#include <memory>

class SDL_Window;
class SDL_Renderer;

namespace engine {

    class Game {
    private:
        std::unique_ptr<SDL_Window, void (*)(SDL_Window *)> m_window;
        std::unique_ptr<SDL_Renderer, void (*)(SDL_Renderer *)> m_renderer;
        bool m_is_running;

        void process_input();
        void update();
        void render();
        void setup();

    public:
        explicit Game(SDL_Window *window, SDL_Renderer *renderer);

        void run();
    };

} // namespace engine

#endif // GAME_HPP
