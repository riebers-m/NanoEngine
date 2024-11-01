//
// Created by HP on 01.11.2024.
//

#include "Commons.hpp"

namespace engine {
    bool Commons::AABB_collision_check(float a_x, float a_y, float a_w, float a_h, float b_x, float b_y, float b_w,
                                       float b_h) {
        return (a_x < b_x + b_w && a_x + a_w > b_x && a_y < b_y + b_h && a_y + a_h > b_h);
    }
} // namespace engine
