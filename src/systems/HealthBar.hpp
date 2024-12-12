//
// Created by HP on 21.11.2024.
//

#ifndef HEALTHBAR_HPP
#define HEALTHBAR_HPP
#include <SDL_rect.h>


#include "ECS/ECS.hpp"

struct SDL_Renderer;

namespace engine {
    class AssetStore;
}

namespace systems {
    class HealthBar : public ecs::System {
        using Logger = std::shared_ptr<engine::Logger>;

    private:
        Logger m_logger;

    public:
        explicit HealthBar(ecs::registry registry,Logger logger);

        void update(SDL_Renderer *, engine::AssetStore *, SDL_Rect const &camera);
    };

} // namespace systems


#endif // HEALTHBAR_HPP
