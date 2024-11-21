//
// Created by HP on 01.11.2024.
//

#include "Commons.hpp"

#include <ios>
#include <iostream>

namespace engine {
    bool Commons::AABB_collision_check(float a_x, float a_y, float a_w, float a_h, float b_x, float b_y, float b_w,
                                       float b_h) {
        auto const check_1 = a_x < b_x + b_w;
        auto const check_2 = a_x + a_w > b_x;
        auto const check_3 = a_y < b_y + b_h;
        auto const check_4 = a_y + a_h > b_y;
        return (check_1 && check_2 && check_3 && check_4);
    }
} // namespace engine
