//
// Created by HP on 11.10.2024.
//

#ifndef TIMER_HPP
#define TIMER_HPP
#include <chrono>

namespace engine {

    class Timer {
    public:
        Timer() = default;

        std::chrono::milliseconds time_passed();
        void start();
        void stop();
        std::chrono::milliseconds round();
        bool has_passed(std::chrono::milliseconds const &delta);

    private:
        std::chrono::system_clock::time_point m_start_time;
        std::chrono::system_clock::time_point m_stop_time;
    };

} // namespace engine

#endif // TIMER_HPP
