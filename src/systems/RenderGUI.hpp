//
// Created by HP on 26.11.2024.
//

#ifndef RENDERGUI_HPP
#define RENDERGUI_HPP
#include <SDL_render.h>


#include "ECS/ECS.hpp"

namespace systems {

    class RenderGUI : public ecs::System {
        using Logger = std::shared_ptr<engine::Logger>;

    private:
        Logger m_logger;

    public:
        explicit RenderGUI(ecs::registry registry, Logger logger);

        void update(ecs::Registry *registry, SDL_Rect const &camera);
    };

} // namespace systems

#endif // RENDERGUI_HPP
