//
// Created by HP on 03.12.2024.
//

#ifndef LEVELLOADER_HPP
#define LEVELLOADER_HPP
#include <SDL_render.h>
#include <sol/state.hpp>


#include "Logger.hpp"
#include "config/Configuration.hpp"

struct SDL_Renderer;

namespace ecs {
    class Registry;

}

namespace engine {
    class AssetStore;

    class LevelLoader {
        using Logger = std::shared_ptr<engine::Logger>;

    private:
        Logger m_logger;

    public:
        explicit LevelLoader(Logger logger);

        [[nodiscard]] Configuration load_level(sol::state &lua, ecs::Registry *, AssetStore *, SDL_Renderer *,
                                               int level_id);
    };

} // namespace engine

#endif // LEVELLOADER_HPP
