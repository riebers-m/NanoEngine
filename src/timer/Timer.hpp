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

        [[nodiscard]] std::chrono::milliseconds time_passed() const;
        void start();
        void stop();
        [[nodiscard]] std::chrono::milliseconds round() const;
        bool has_passed(std::chrono::milliseconds const &delta) const;

    private:
        std::chrono::high_resolution_clock::time_point m_start_time;
        std::chrono::high_resolution_clock::time_point m_stop_time;
    };

} // namespace engine

#endif // TIMER_HPP
