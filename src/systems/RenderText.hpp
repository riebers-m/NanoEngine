//
// Created by HP on 21.11.2024.
//

#ifndef RENDERTEXT_HPP
#define RENDERTEXT_HPP

#include <SDL_rect.h>


#include "ECS/ECS.hpp"

namespace engine {
    class AssetStore;
}

struct SDL_Renderer;

namespace systems {
    class RenderText : public ecs::System {
        using Logger = std::shared_ptr<spdlog::logger>;

    private:
        Logger m_logger;

    public:
        explicit RenderText(Logger logger);

        void update(SDL_Renderer *, engine::AssetStore *, SDL_Rect const &camera);
    };

} // namespace systems


#endif // RENDERTEXT_HPP
