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

        explicit Transform(glm::vec2 const &position = {0, 0}, glm::vec2 const scale = {1.0, 1.0},
                           degree rotation = 0) : position{position}, scale{scale}, rotation{rotation} {}
    };
} // namespace component

#endif // TRANSFORM_HPP
