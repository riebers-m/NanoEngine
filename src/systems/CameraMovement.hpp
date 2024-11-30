//
// Created by HP on 03.11.2024.
//

#ifndef CAMERAMOVEMENT_HPP
#define CAMERAMOVEMENT_HPP
#include <SDL_rect.h>


#include "ECS/ECS.hpp"
#include "config/Configuration.hpp"
namespace systems {

    class CameraMovement : public ecs::System {
        using Logger = std::shared_ptr<engine::Logger>;

    private:
        Logger m_logger;

    public:
        explicit CameraMovement(Logger logger);

        void update(SDL_Rect &camera, engine::Configuration const &config);
    };

} // namespace systems

#endif // CAMERAMOVEMENT_HPP
