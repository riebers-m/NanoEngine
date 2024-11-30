//
// Created by HP on 30.11.2024.
//

#ifndef SPDLOGLOGGER_HPP
#define SPDLOGLOGGER_HPP
#include "Logger.hpp"

#include <utility>
#include "spdlog/logger.h"

namespace engine {
    class Spdlogger : public ILogger {
    private:
        std::shared_ptr<spdlog::logger> m_logger;

    public:
        explicit Spdlogger(std::shared_ptr<spdlog::logger> logger) : m_logger(std::move(logger)) {}

        void debug(std::string const &message) override { m_logger->debug(message); }
        void info(std::string const &message) override { m_logger->info(message); }
        void warn(std::string const &message) override { m_logger->warn(message); }
        void error(std::string const &message) override { m_logger->error(message); }
        void draw(std::string const &title) override{};
    };
} // namespace engine
#endif // SPDLOGLOGGER_HPP
