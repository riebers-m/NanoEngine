//
// Created by HP on 31.10.2024.
//

#ifndef ANIMATION_HPP
#define ANIMATION_HPP
#include <chrono>

namespace component {
    enum class animate {
        infinite,
        once,
    };
    struct Animation {
        uint8_t num_frames;
        uint8_t current_frame;
        uint8_t frame_speed_rate; // how many frames shall be rendered per second
        animate loops;
        float elapsed_time;

        explicit Animation(int num_frames = 1, int frame_speed_rate = 1, animate loops = animate::infinite) :
            num_frames{static_cast<uint8_t>(num_frames)}, current_frame{1},
            frame_speed_rate{static_cast<uint8_t>(frame_speed_rate)}, loops(loops), elapsed_time{0} {}
    };
} // namespace component

#endif // ANIMATION_HPP
