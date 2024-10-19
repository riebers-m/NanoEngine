//
// Created by HP on 19.10.2024.
//

#ifndef RIGIDBODY_HPP
#define RIGIDBODY_HPP
#include "glm/vec2.hpp"

namespace component {
    struct RigidBody {
        glm::vec2 velocity{};
    };
} // namespace component
#endif // RIGIDBODY_HPP
