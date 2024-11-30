//
// Created by HP on 30.11.2024.
//

#ifndef IMGUILOGWINDOW_HPP
#define IMGUILOGWINDOW_HPP
#include "Logger.hpp"

namespace engine {
    class ImGuiLogWindow : public ILogger {
        struct AppLog;

    private:
        std::unique_ptr<AppLog> m_logger;

    public:
        ImGuiLogWindow();

        void debug(const std::string &message) override;
        void info(const std::string &message) override;
        void warn(const std::string &message) override;
        void error(const std::string &message) override;
        void draw(std::string const &title) override;
    };
} // namespace engine
#endif // IMGUILOGWINDOW_HPP
