//
// Created by HP on 03.12.2024.
//

#ifndef LEVELLOADER_HPP
#define LEVELLOADER_HPP
#include <sol/state.hpp>


#include "config/Configuration.hpp"

struct SDL_Renderer;

namespace ecs {
    class Registry;

}

namespace engine {
    class AssetStore;

    class LevelLoader {
    public:
        [[nodiscard]] Configuration load_level(sol::state &lua, ecs::Registry *, AssetStore *, SDL_Renderer *,
                                               int level);
    };

} // namespace engine

#endif // LEVELLOADER_HPP
