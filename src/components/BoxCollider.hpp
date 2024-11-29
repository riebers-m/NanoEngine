//
// Created by HP on 31.10.2024.
//

#ifndef BOXCOLIDER_HPP
#define BOXCOLIDER_HPP
#include <cstdint>

#include "glm/vec2.hpp"
namespace component {
    struct BoxCollider {
        uint16_t width;
        uint16_t height;
        glm::vec2 offset;

        explicit BoxCollider(int width = 0, int height = 0, glm::vec2 const &offset = {}) :
            width{static_cast<uint16_t>(width)}, height{static_cast<uint16_t>(height)}, offset{offset} {}
    };
} // namespace component
#endif // BOXCOLIDER_HPP
