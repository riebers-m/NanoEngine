#include <cpptrace/from_current.hpp>
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

    CPPTRACE_TRY {
        engine::Game game = engine::Engine::create(logger);

        game.run();
        return 0;
    }
    CPPTRACE_CATCH(std::exception const &e) {
        logger->error(e.what());
        cpptrace::from_current_exception().print();
        return 1;
    }
}
