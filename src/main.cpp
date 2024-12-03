#include "common/ImGuiLogWindow.hpp"
#include "common/SpdlogLogger.hpp"
#include "const/Const.hpp"
#include "game/Engine.hpp"
#include "spdlog/spdlog.h"

int main(int argc, char *argv[]) {
    // Use spdlog logger
    // auto spd_logger = spdlog::stdout_color_mt("console", spdlog::color_mode::automatic);
    // spd_logger->set_level(spdlog::level::debug);
    // auto logger = std::make_shared<engine::Logger>(std::make_unique<engine::Spdlogger>(spd_logger));

    // Use ImGui Debug console
    auto logger = std::make_shared<engine::Logger>(std::make_unique<engine::ImGuiLogWindow>());
    try {
        engine::Game game = engine::Engine::create(logger);

        game.run();
        return 0;

    } catch (std::exception const &e) {
        logger->error(e.what());
        return 1;
    }
}
