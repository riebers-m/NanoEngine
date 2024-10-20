#include <iostream>
#include "game/Engine.hpp"

int main(int argc, char *argv[]) {
    try {
        engine::Game game = engine::Engine::create();

        game.run();
        return 0;

    } catch (std::exception const &e) {
        std::cout << e.what();
        return 1;
    }
}
