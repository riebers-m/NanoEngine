//
// Created by HP on 08.11.2024.
//

#ifndef PROJECTILEEMITTER_HPP
#define PROJECTILEEMITTER_HPP
#include "ECS/ECS.hpp"
#include "events/KeyPressedEvent.hpp"

namespace events {
    class EventBus;
}
namespace systems {

    class ProjectileEmitter : public ecs::System {
        using Logger = std::shared_ptr<engine::Logger>;

    private:
        Logger m_logger;
        ecs::Registry *m_registry;

    public:
        explicit ProjectileEmitter(Logger logger, ecs::Registry *);

        void update();
        void subscribe_to_event(events::EventBus *);
        void on_keypress(events::KeyPressedEvent &event);
    };

} // namespace systems

#endif // PROJECTILEEMITTER_HPP
