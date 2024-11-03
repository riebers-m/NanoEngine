//
// Created by HP on 03.11.2024.
//

#ifndef KEYBOARDCONTROLLED_HPP
#define KEYBOARDCONTROLLED_HPP
#include "glm/vec2.hpp"
namespace component {
    struct KeyboardControlled {
        glm::vec2 up_velocity;
        glm::vec2 right_velocity;
        glm::vec2 down_velocity;
        glm::vec2 left_velocity;

        explicit KeyboardControlled(glm::vec2 const &up_velocity = {}, glm::vec2 const &right_velocity = {},
                                    glm::vec2 const &down_velocity = {}, glm::vec2 const &left_velocity = {}) :
            up_velocity{up_velocity}, right_velocity{right_velocity}, down_velocity{down_velocity},
            left_velocity{left_velocity} {}
    };
} // namespace component
#endif // KEYBOARDCONTROLLED_HPP
