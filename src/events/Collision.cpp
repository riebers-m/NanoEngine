//
// Created by HP on 01.11.2024.
//

#include "Collision.hpp"

namespace events {
    Collision::Collision(ecs::Entity a, ecs::Entity b) : a{a}, b{b} {}
} // namespace events
