//
// Created by HP on 26.11.2024.
//

#ifndef RENDERGUI_HPP
#define RENDERGUI_HPP
#include <SDL_render.h>


#include "ECS/ECS.hpp"

namespace systems {

    class RenderGUI : public ecs::System {
        using Logger = std::shared_ptr<spdlog::logger>;

    private:
        Logger m_logger;

    public:
        explicit RenderGUI(Logger logger);

        void update(SDL_Renderer *renderer, ecs::Registry *registry, SDL_Rect const &camera);
    };

} // namespace systems

#endif // RENDERGUI_HPP
