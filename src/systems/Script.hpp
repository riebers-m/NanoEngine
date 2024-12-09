//
// Created by HP on 09.12.2024.
//

#pragma once
#include "ECS/ECS.hpp"

namespace systems {
    class Script : public ecs::System {
    private:
    public:
        explicit Script();

        void update();
    };

} // namespace systems
