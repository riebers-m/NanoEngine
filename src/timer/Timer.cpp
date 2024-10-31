//
// Created by HP on 11.10.2024.
//

#include "timer/Timer.hpp"

namespace engine {
    std::chrono::milliseconds Timer::time_passed() const {
        return std::chrono::duration_cast<std::chrono::milliseconds>(m_stop_time - m_start_time);
    }
    void Timer::start() { m_start_time = std::chrono::high_resolution_clock::now(); }
    void Timer::stop() { m_stop_time = std::chrono::high_resolution_clock::now(); }
    std::chrono::milliseconds Timer::round() const {
        return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() -
                                                                     m_start_time);
    }
    bool Timer::has_passed(std::chrono::milliseconds const &delta) const { return round() >= delta; }
} // namespace engine
