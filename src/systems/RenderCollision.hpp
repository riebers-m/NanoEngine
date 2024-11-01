//
// Created by HP on 01.11.2024.
//

#ifndef RENDERCOLLISION_HPP
#define RENDERCOLLISION_HPP
#include "ECS/ECS.hpp"

struct SDL_Renderer;

namespace systems {

    class RenderCollision : public ecs::System {
        using Logger = std::shared_ptr<spdlog::logger>;

    private:
        Logger m_logger;

    public:
        explicit RenderCollision(Logger logger);

        void update(SDL_Renderer *);
    };

} // namespace systems

#endif // RENDERCOLLISION_HPP
