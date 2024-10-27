//
// Created by HP on 18.10.2024.
//

#ifndef TRANSFORM_HPP
#define TRANSFORM_HPP
#include <glm/glm.hpp>
namespace component {
    using degree = double;
    struct Transform {
        glm::vec2 position;
        glm::vec2 scale;
        degree rotation; // angle
    };
} // namespace component

#endif // TRANSFORM_HPP
