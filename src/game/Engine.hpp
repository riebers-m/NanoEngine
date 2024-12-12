//
// Created by HP on 10.10.2024.
//

#ifndef ENGINE_HPP
#define ENGINE_HPP
#include "common/Logger.hpp"
#include "game/Game.hpp"

namespace engine {
    class Engine {
    private:
        Engine();

        static Engine const &instance();

    public:
        Engine(Engine const &) = delete;
        Engine(Engine &&) = delete;
        Engine operator=(Engine const &) = delete;
        Engine operator=(Engine &&) = delete;

        ~Engine();
        static Game create(std::shared_ptr<Logger> logger, Engine const & = instance());
    };

} // namespace engine

#endif // ENGINE_HPP
