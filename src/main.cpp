#include <iostream>
#include "game/Engine.hpp"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/spdlog.h"

int main(int argc, char *argv[]) {
    auto logger = spdlog::stdout_color_mt("console", spdlog::color_mode::automatic);
    try {
        engine::Game game = engine::Engine::create(logger);

        game.run();
        return 0;

    } catch (std::exception const &e) {
        logger->error(e.what());
        return 1;
    }
}
