//
// Created by HP on 20.10.2024.
//

#ifndef RENDERSYSTEM_HPP
#define RENDERSYSTEM_HPP
#include "ECS/ECS.hpp"

class SDL_Renderer;

namespace systems {
    class RenderSystem : public ecs::System {
        using Logger = std::shared_ptr<spdlog::logger>;

    private:
        Logger m_logger;

    public:
        explicit RenderSystem(Logger);
        void update(SDL_Renderer *) const;
    };
} // namespace systems


#endif // RENDERSYSTEM_HPP
