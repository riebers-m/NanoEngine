//
// Created by HP on 01.11.2024.
//

#ifndef COMMONS_HPP
#define COMMONS_HPP

namespace engine {

    class Commons {
    public:
        static bool AABB_collision_check(float a_x, float a_y, float a_w, float a_h, float b_x, float b_y, float b_w,
                                         float b_h);
    };

} // namespace engine

#endif // COMMONS_HPP
