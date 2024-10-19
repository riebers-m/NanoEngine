#include <iostream>
#include "game/Engine.hpp"

int main(int argc, char *argv[]) {
    try {
        engine::Game game = engine::Engine::create();

        game.run();
        return 0;

    } catch (std::runtime_error const &e) {
        std::cout << e.what();
        return 1;
    }
}
