//
// Created by HP on 20.10.2024.
//

#ifndef RENDERSYSTEM_HPP
#define RENDERSYSTEM_HPP
#include <SDL_render.h>


#include "ECS/ECS.hpp"

namespace engine {
    class AssetStore;
}
struct SDL_Renderer;

namespace systems {
    class RenderSystem : public ecs::System {
        using Logger = std::shared_ptr<engine::Logger>;

    private:
        Logger m_logger;

    public:
        explicit RenderSystem(ecs::registry registry, Logger);
        void update(SDL_Renderer *, engine::AssetStore const *asset_store, SDL_Rect const &camera) const;
    };
} // namespace systems


#endif // RENDERSYSTEM_HPP
